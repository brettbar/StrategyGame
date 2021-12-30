#include "map.hpp"
#include <raylib.h>

namespace Map
{


  void UpdateProvinces(entt::registry &registry)
  {
//    auto tilesView = registry.view<TileMap>();
//    auto tilesEntity = tilesView.front();
//    TileMap &tileMap = tilesView.get<TileMap>(tilesEntity);
//
//    for (Tile *tilePtr: tileMap.tiles)
//    {
//      if (tilePtr == nullptr)
//        continue;
//      Tile &tile = *tilePtr;
//
//      if (tile.owner > -1 && tile.population >= 0)
//        tile.population += 25;
//    }
  }


  void SetProvinceOwner(entt::registry &registry, u32 owner, Vector2 clickPos)
  {
//    i32 tileId = determineTileIdFromClick(clickPos);
//    assert(tileId >= 0);
//
//    auto tilesView = registry.view<TileMap>();
//    auto tilesEntity = tilesView.front();
//    TileMap &tileMap = tilesView.get<TileMap>(tilesEntity);
//
//    for (Tile *tilePtr: tileMap.tiles)
//    {
//      if (tilePtr == nullptr)
//        continue;
//      Tile &tile = *tilePtr;
//
//      if (tile.id == (u32) tileId)
//      {
//        tile.owner = owner;
//        switch (tile.owner)
//        {
//          case 0:
//            tile.name = "Rome";
//            break;
//          case 1:
//            tile.name = "Athens";
//            break;
//          case 2:
//            tile.name = "Lugudunon";
//            break;
//          case 3:
//            tile.name = "Carthage";
//            break;
//          case 4:
//            tile.name = "Persepolis";
//            break;
//        }
//      }
//    }
  }

  void DrawProvinces(entt::registry &reg, TextureCache &cache)
  {
//    auto tilesView = reg.view<TileMap>();
//    auto tilesEntity = tilesView.front();
//    TileMap &tileMap = tilesView.get<TileMap>(tilesEntity);
//
//    for (Tile *tilePtr: tileMap.tiles)
//    {
//      if (tilePtr == nullptr)
//        continue;
//      Tile &tile = *tilePtr;
//      // str idString = std::to_string(tile.id);
//      // const char *idText = idString.c_str();
//      // if (debug)
//      //   DrawText(idText, tile.position.x + 16.0, tile.position.y + 16.0,
//      //   14,
//      //            WHITE);
//
//      // str ownerString = std::to_string(tile.owner);
//      // const char *ownertext = ownerString.c_str();
//      // DrawText(ownertext, tile.position.x + 48.0, tile.position.y + 16.0,
//      // 14,
//      //          BLUE);
//
//      //       str coordString = std::to_string((u32)tile.coord.x) + "," +
//      //                         std::to_string((u32)tile.coord.y);
//      //       const char *coordText = coordString.c_str();
//      ////       if (.debug)
//      //         DrawText(coordText, tile.position.x + 16.0, tile.position.y + 16.0, 14,
//      //                  BLUE);
//
//      // str popString = std::to_string(tile.population);
//      // const char *text = popString.c_str();
//      // if (debug)
//      //   DrawText(text, tile.position.x + 16.0, tile.position.y + 32.0,
//      //   14,
//      //            RED);
//
//      if (tile.owner > -1)
//      {
//        Color color = BLACK;
//
//        Rectangle frameRec = {0.0, 0.0, 128, 128.0};
//
//        switch (tile.owner)
//        {
//          case 0:
//            color = RED;
//            frameRec.x = 0.0f;
//            break;
//          case 1:
//            color = SKYBLUE;
//            frameRec.x = 128.0f;
//            break;
//          case 2:
//            color = GREEN;
//            frameRec.x = 256.0f;
//            break;
//          case 3:
//            color = PURPLE;
//            frameRec.x = 384.0f;
//            break;
//          case 4:
//            color = ORANGE;
//            frameRec.x = 513.0f;
//            break;
//        }
//
//        DrawTextureRec(cache.handle(hstr{"factionOverlay"})->texture,
//                       frameRec, tile.position, Fade(WHITE, 0.5));
//      }
//
//      if (tile.population >= 100)
//      {
//        // DrawSingleBorder(tile);
//        DrawTextureV(cache.handle(hstr{"romanVillageTexture"})->texture,
//                     tile.position, WHITE);
//        if (tile.name != "")
//          DrawText(tile.name.c_str(), tile.position.x + 50.0,
//                   tile.position.y + 86.0, 14, WHITE);
//      }
//    }
  }

  //  Tile *FindTileByCoord(TileMap &tileMap, u32 x, u32 y)
  //  {
  //
  //    if ((x >= 0 && x <= MAP_WIDTH) && (y >= 0 && y <= MAP_HEIGHT))
  //    {
  //      return tileMap.tiles[index(x, y)];
  //    }
  //    return nullptr;
  //  }


};// namespace Map
