#include "map.hpp"

#include <raylib.h>

namespace Map
{

void CreateTerrain(entt::registry &registry, u32 mapWidth, u32 mapHeight)
{
  entt::entity entity = registry.create();
  TileMap tileMap = {
    .tiles = {},
  };

  for (u32 i = 0; i < mapWidth * mapHeight; i++)
  {
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

    if (y % 2 == 1) tile->position = {xPos + 64, yPos};
    else
      tile->position = {xPos, yPos};

    tile->center = {tile->position.x + 64, tile->position.y + 64};
    tile->coord = {x, y};
    tileMap.tiles[index(x, y)] = *tile;
  }
  registry.emplace<TileMap>(entity, tileMap);
}

u32 index(u32 x, u32 y) { return x + MAP_WIDTH * y; };

void UpdateProvinces(entt::registry &registry)
{
  auto tilesView = registry.view<TileMap>();
  auto tilesEntity = tilesView.front();
  TileMap &tileMap = tilesView.get<TileMap>(tilesEntity);

  for (Tile &tile: tileMap.tiles)
  {
    if (tile.owner > -1 && tile.population >= 0) tile.population += 25;
  }
}

void DrawTerrain(entt::registry &registry, Texture2D hex, Rectangle frameRec)
{
  auto tilesView = registry.view<TileMap>();
  auto tilesEntity = tilesView.front();
  TileMap &tileMap = tilesView.get<TileMap>(tilesEntity);

  for (Tile &tile: tileMap.tiles)
  {
    DrawTextureRec(hex, frameRec, tile.position, WHITE);
  }
}

void SetProvinceOwner(entt::registry &registry, u32 owner, Vector2 clickPos)
{
  i32 tileId = determineTileIdFromClick(clickPos);
  assert(tileId >= 0);

  auto tilesView = registry.view<TileMap>();
  auto tilesEntity = tilesView.front();
  TileMap &tileMap = tilesView.get<TileMap>(tilesEntity);

  for (Tile &tile: tileMap.tiles)
  {
    if (tile.id == (u32) tileId)
    {
      tile.owner = owner;
      switch (tile.owner)
      {
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

void DrawProvinces(entt::registry &reg, TextureCache &cache)
{
  auto tilesView = reg.view<TileMap>();
  auto tilesEntity = tilesView.front();
  TileMap &tileMap = tilesView.get<TileMap>(tilesEntity);

  for (Tile &tile: tileMap.tiles)
  {
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

    if (tile.owner > -1)
    {
      Color color = BLACK;

      Rectangle frameRec = {0.0, 0.0, 128, 128.0};

      switch (tile.owner)
      {
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

      DrawTextureRec(cache.handle(hstr{"factionOverlay"})->texture, frameRec,
                     tile.position, Fade(WHITE, 0.5));
    }

    if (tile.population >= 100)
    {
      // DrawSingleBorder(tile);
      DrawTextureV(cache.handle(hstr{"romanVillageTexture"})->texture,
                   tile.position, WHITE);
      if (tile.name != "")
        DrawText(tile.name.c_str(), tile.position.x + 50.0,
                 tile.position.y + 86.0, 14, WHITE);
    }
  }
}

Tile *FindTileByCoord(TileMap &tileMap, u32 x, u32 y)
{

  if ((x >= 0 && x <= MAP_WIDTH) && (y >= 0 && y <= MAP_HEIGHT))
  {
    return &tileMap.tiles[index(x, y)];
  }
  return nullptr;
}
};// namespace Map
