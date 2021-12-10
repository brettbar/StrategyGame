#include "map.hpp"
#include <raylib.h>

namespace Map {

void CreateTerrain(entt::registry &registry, u32 mapWidth, u32 mapHeight) {
  entt::entity entity = registry.create();
  TileMap tileMap = {
      .tiles = {},
  };

  for (u32 i = 0; i < mapWidth * mapHeight; i++) {
    Tile *tile = new Tile{};
    // tile.id = x + y * 128;
    tile->id = i;
    tile->owner = -1;
    tile->population = 0;
    tile->name = "";

    u32 x = i % mapWidth;
    u32 y = i / mapHeight;

    f32 xPos = x * 128;
    f32 yPos = y * 96;

    if (y % 2 == 1)
      tile->position = {xPos + 64, yPos};
    else
      tile->position = {xPos, yPos};

    tile->center = {tile->position.x + 64, tile->position.y + 64};

    tile->coord = {x, y};

    tileMap.tiles[index(x, y)] = tile;

    // tile->neighbors = {
    //     tilemap.tiles[tile->coord.y - 1][tile->coord.x + 1],
    //     tilemap.tiles[tile->coord.y][tile->coord.x + 1],
    //     tilemap.tiles[tile->coord.y + 1][tile->coord.x + 1],
    //     tilemap.tiles[tile->coord.y + 1][tile->coord.x],
    //     tilemap.tiles[tile->coord.y][tile->coord.x - 1],
    //     tilemap.tiles[tile->coord.y - 1][tile->coord.x],
    // };
  }
  registry.emplace<TileMap>(entity, tileMap);
}

u32 index(u32 x, u32 y) { return x + MAP_WIDTH * y; };

void UpdateProvinces(entt::registry &registry) {
  auto tilesView = registry.view<TileMap>();
  auto tilesEntity = tilesView.front();
  TileMap &tileMap = tilesView.get<TileMap>(tilesEntity);

  for (Tile *tile : tileMap.tiles) {
    if (tile != nullptr)
      if (tile->owner > -1 && tile->population >= 0)
        tile->population += 25;
  }
}

void DrawTerrain(entt::registry &registry, Texture2D hex, Rectangle frameRec) {
  auto tilesView = registry.view<TileMap>();
  auto tilesEntity = tilesView.front();
  TileMap &tileMap = tilesView.get<TileMap>(tilesEntity);

  for (Tile *tile : tileMap.tiles) {
    if (tile != nullptr)
      DrawTextureRec(hex, frameRec, tile->position, WHITE);
  }
}

void SetProvinceOwner(entt::registry &registry, u32 owner, Vector2 clickPos) {
  i32 tileId = determineTileIdFromClick(clickPos);
  assert(tileId >= 0);

  auto tilesView = registry.view<TileMap>();
  auto tilesEntity = tilesView.front();
  TileMap &tileMap = tilesView.get<TileMap>(tilesEntity);

  for (Tile *tile : tileMap.tiles) {
    if (tile != nullptr)
      if (tile->id == (u32)tileId) {
        tile->owner = owner;
        switch (tile->owner) {
        case 0:
          tile->name = "Rome";
          break;
        case 1:
          tile->name = "Athens";
          break;
        case 2:
          tile->name = "Lugudunon";
          break;
        case 3:
          tile->name = "Carthage";
          break;
        case 4:
          tile->name = "Persepolis";
          break;
        }
      }
  }
}

void DrawProvinces(State &state) {
  auto tilesView = state.registry.view<TileMap>();
  auto tilesEntity = tilesView.front();
  TileMap &tileMap = tilesView.get<TileMap>(tilesEntity);

  for (Tile *tilePtr : tileMap.tiles) {
    if (tilePtr == nullptr)
      continue;
    Tile &tile = *tilePtr;
    // str idString = std::to_string(tile.id);
    // const char *idText = idString.c_str();
    // if (debug)
    //   DrawText(idText, tile.position.x + 16.0, tile.position.y + 16.0,
    //   14,
    //            WHITE);

    // str ownerString = std::to_string(tile.owner);
    // const char *ownertext = ownerString.c_str();
    // DrawText(ownertext, tile.position.x + 48.0, tile.position.y + 16.0,
    // 14,
    //          BLUE);

    // str coordString = std::to_string((u32)tile.coord.x) + "," +
    //                   std::to_string((u32)tile.coord.y);
    // const char *coordText = coordString.c_str();
    // if (debug)
    //   DrawText(coordText, tile.position.x + 16.0, tile.position.y + 16.0, 14,
    //            BLUE);

    // str popString = std::to_string(tile.population);
    // const char *text = popString.c_str();
    // if (debug)
    //   DrawText(text, tile.position.x + 16.0, tile.position.y + 32.0,
    //   14,
    //            RED);

    if (tile.owner > -1) {
      Color color = BLACK;

      Rectangle frameRec = {0.0, 0.0, 128, 128.0};

      switch (tile.owner) {
      case 0:
        color = RED;
        frameRec.x = 0.0f;
        break;
      case 1:
        color = SKYBLUE;
        frameRec.x = 128.0f;
        break;
      case 2:
        color = GREEN;
        frameRec.x = 256.0f;
        break;
      case 3:
        color = PURPLE;
        frameRec.x = 384.0f;
        break;
      case 4:
        color = ORANGE;
        frameRec.x = 513.0f;
        break;
      }

      DrawTextureRec(state.textures.at("factionOverlay"), frameRec,
                     tile.position, Fade(WHITE, 0.5));
    }

    if (tile.population >= 100) {
      // DrawSingleBorder(tile);
      DrawTextureV(state.textures.at("romanVillageTexture"), tile.position,
                   WHITE);
      if (tile.name != "")
        DrawText(tile.name.c_str(), tile.position.x + 50.0,
                 tile.position.y + 86.0, 14, WHITE);
    }
  }
}

Tile *FindTileByCoord(TileMap &tileMap, u32 x, u32 y) {

  if ((x >= 0 && x <= MAP_WIDTH) && (y >= 0 && y <= MAP_HEIGHT)) {
    return tileMap.tiles[index(x, y)];
  }
  return nullptr;
}

i32 determineTileIdFromClick(Vector2 clickPos) {
  Vector2 *target = determineTilePos(clickPos);
  if (target == nullptr)
    return -1;

  i32 row = target->y / 96.0;
  i32 column;

  if (row % 2 == 1) {
    column = (target->x - 64.0) / 128.0;
  } else {
    column = target->x / 128.0;
  }

  return column + row * 128;
}

Vector2 *determineTilePos(Vector2 inputPos) {
  i32 x = inputPos.x;
  i32 y = inputPos.y;
  i32 gridHeight = 96;
  i32 gridWidth = 128;
  i32 halfWidth = gridWidth / 2;

  // Find the row and column of the box that the point falls in.
  int row = (int)(y / gridHeight);
  int column;

  bool rowIsOdd = row % 2 == 1;

  // Is the row an odd number?
  if (rowIsOdd) // Yes: Offset x to match the indent of the row
    column = (int)((x - halfWidth) / gridWidth);
  else // No: Calculate normally
    column = (int)(x / gridWidth);

  // Work out the position of the point relative to the box it is in
  f32 relY = y - (row * gridHeight);
  f32 relX;

  if (rowIsOdd)
    relX = (x - (column * gridWidth)) - halfWidth;
  else
    relX = x - (column * gridWidth);

  f32 c = 32.0;
  f32 m = c / halfWidth;

  // Work out if the point is above either of the hexagon's top edges
  if (relY < (-m * relX) + c) { // LEFT edge
    row--;
    if (!rowIsOdd)
      column--;
  } else if (relY < (m * relX) - c) { // RIGHT edge
    row--;
    if (rowIsOdd)
      column++;
  }

  f32 tileOrigX = (column * 128.0) + 64.0;
  f32 tileOrigY = (row * 96.0) + 64.0;

  if (row % 2 == 1) {
    tileOrigX += 64.0;
  }
  return new Vector2{tileOrigX, tileOrigY};
}
}; // namespace Map
