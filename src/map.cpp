#include "map.hpp"
#include <raylib.h>

namespace Map {

void CreateTerrain(entt::registry &registry, u32 mapWidth, u32 mapHeight) {
  for (u32 i = 0; i < mapWidth * mapHeight; i++) {
    entt::entity entity = registry.create();
    Tile tile = Tile{};
    // tile.id = x + y * 128;
    tile.id = i;
    tile.owner = -1;
    tile.population = 0;
    tile.name = "";

    u32 x = i % mapWidth;
    u32 y = i / mapHeight;

    f32 xPos = x * 128;
    f32 yPos = y * 96;

    if (y % 2 == 1)
      tile.position = {xPos + 64, yPos};
    else
      tile.position = {xPos, yPos};

    tile.center = {tile.position.x + 64, tile.position.y + 64};

    tile.coord = {x, y};

    // tile->neighbors = {
    //     tilemap.tiles[tile->coord.y - 1][tile->coord.x + 1],
    //     tilemap.tiles[tile->coord.y][tile->coord.x + 1],
    //     tilemap.tiles[tile->coord.y + 1][tile->coord.x + 1],
    //     tilemap.tiles[tile->coord.y + 1][tile->coord.x],
    //     tilemap.tiles[tile->coord.y][tile->coord.x - 1],
    //     tilemap.tiles[tile->coord.y - 1][tile->coord.x],
    // };
    registry.emplace<Tile>(entity, tile);
  }
}

u32 index(u32 x, u32 y, u32 mapWidth) { return x + mapWidth * y; };

void UpdateProvinces(entt::registry &registry) {
  auto tilesView = registry.view<Tile>();

  for (auto entity : tilesView) {
    Tile &tile = tilesView.get<Tile>(entity);
    if (tile.owner > -1 && tile.population >= 0)
      tile.population += 25;
  }
}

void DrawTerrain(entt::registry &registry, Texture2D hex, Rectangle frameRec) {
  auto tilesView = registry.view<Tile>();

  for (auto entity : tilesView) {
    Tile &tile = tilesView.get<Tile>(entity);
    DrawTextureRec(hex, frameRec, tile.position, WHITE);
  }
}

void SetProvinceOwner(entt::registry &registry, u32 owner, Vector2 clickPos) {
  i32 tileId = determineTileIdFromClick(clickPos);
  assert(tileId >= 0);

  auto tilesView = registry.view<Tile>();

  for (auto entity : tilesView) {
    Tile &tile = tilesView.get<Tile>(entity);
    if (tile.id == (u32)tileId) {
      tile.owner = owner;
      switch (tile.owner) {
      case 0:
        tile.name = "Rome";
        break;
      case 1:
        tile.name = "Athens";
        break;
      case 2:
        tile.name = "Lugudunon";
        break;
      case 3:
        tile.name = "Carthage";
        break;
      case 4:
        tile.name = "Persepolis";
        break;
      }
    }
  }
}

void DrawProvinces(entt::registry &registry, bool debug, Texture2D village) {
  auto tilesView = registry.view<Tile>();
  for (auto entity : tilesView) {
    Tile &tile = tilesView.get<Tile>(entity);
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

    if (tile.population >= 100) {
      // DrawSingleBorder(tile);
      DrawTextureV(village, tile.position, WHITE);
      if (tile.name != "")
        DrawText(tile.name.c_str(), tile.position.x + 50.0,
                 tile.position.y + 86.0, 14, WHITE);

      if (tile.owner > -1 && tile.population > 100) {
        // f32 centerX = tile.position.x + 64;
        // f32 centerY = tile.position.y + 64;
        Color color = BLACK;

        switch (tile.owner) {
        case 0:
          color = RED;
          break;
        case 1:
          color = SKYBLUE;
          break;
        case 2:
          color = GREEN;
          break;
        case 3:
          color = PURPLE;
          break;
        case 4:
          color = ORANGE;
          break;
        }

        Vector2 vertices[6] = {
            {tile.center.x, tile.center.y - 64},
            {tile.center.x + 64, tile.center.y - 32},
            {tile.center.x + 64, tile.center.y + 32},
            {tile.center.x, tile.center.y + 64},
            {tile.center.x - 64, tile.center.y + 32},
            {tile.center.x - 64, tile.center.y - 32},
        };

        int topColOffset = 1;
        int botColOffset = 0;
        if (tile.coord.y % 2 == 0) {
          topColOffset = 0;
          botColOffset = 1;
        }

        Tile *neighborNE =
            FindTileByCoord(registry, tile.coord.x + topColOffset, tile.coord.y - 1);
        Tile *neighborE =
            FindTileByCoord(registry, tile.coord.x + 1, tile.coord.y);
        Tile *neighborSE =
            FindTileByCoord(registry, tile.coord.x + topColOffset, tile.coord.y + 1);
        Tile *neighborSW =
            FindTileByCoord(registry, tile.coord.x - botColOffset, tile.coord.y + 1);
        Tile *neighborW =
            FindTileByCoord(registry, tile.coord.x - 1, tile.coord.y);
        Tile *neighborNW =
            FindTileByCoord(registry, tile.coord.x - botColOffset, tile.coord.y - 1);

        if (neighborNE != nullptr) {
          if (neighborNE->owner != tile.owner) {
            // DrawCircleV(neighborNE->center, 16, BLACK);
            DrawLineEx(vertices[0], vertices[1], 2, Fade(color, 0.5f));
          }
        }
        if (neighborE != nullptr) {
          if (neighborE->owner != tile.owner) {
            // DrawCircleV(neighborE->center, 16, BLACK);
            DrawLineEx(vertices[1], vertices[2], 2, Fade(color, 0.5f));
          }
        }
        if (neighborSE != nullptr) {
          if (neighborSE->owner != tile.owner) {
            // DrawCircleV(neighborSE->center, 16, BLACK);
            DrawLineEx(vertices[2], vertices[3], 2, Fade(color, 0.5f));
          }
        }
        if (neighborSW != nullptr) {
          if (neighborSW->owner != tile.owner) {
            // DrawCircleV(neighborSW->center, 16, BLACK);
            DrawLineEx(vertices[3], vertices[4], 2, Fade(color, 0.5f));
          }
        }
        if (neighborW != nullptr) {
          if (neighborW->owner != tile.owner) {
            // DrawCircleV(neighborW->center, 16, BLACK);
            DrawLineEx(vertices[4], vertices[5], 2, Fade(color, 0.5f));
          }
        }
        if (neighborNW != nullptr) {
          if (neighborNW->owner != tile.owner) {
            // DrawCircleV(neighborNW->center, 16, BLACK);
            DrawLineEx(vertices[5], vertices[0], 2, Fade(color, 0.5f));
          }
        }
      }
    }

    // DrawBorders(tilemap);
  }
}

Tile *FindTileByCoord(entt::registry &registry, u32 x, u32 y) {
  auto tilesView = registry.view<Tile>();

  for (auto entity : tilesView) {
    Tile &tile = tilesView.get<Tile>(entity);
    if (tile.coord.x == x && tile.coord.y == y) {
      return &tile;
    }
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
