#include "map.hpp"
#include <raylib.h>

namespace Map
{

  void InitProvinces(entt::registry &reg)
  {
    auto tView = reg.view<Terrain::TileMap>();
    Terrain::TileMap tiles = tView.get<Terrain::TileMap>(tView.front());

    entt::entity entity = reg.create();
    Provinces provinces;

    for (u32 i = 0; i < Terrain::MAP_WIDTH * Terrain::MAP_HEIGHT; i++)
    {
      provinces[i] = std::make_shared<Province>(Province{
        .id = i,
        .owner = -1,
        .population = 0,
        .name = "",
        .tile = tiles[i]
      });
    }

    reg.emplace<Provinces>(entity, provinces);
  }

  void UpdateProvinces(entt::registry &registry)
  {
    auto view = registry.view<Provinces>();
    Provinces &provinces = view.get<Provinces>(view.front());

    for (std::shared_ptr<Province> prov: provinces)
    {
      if (prov->owner > -1 && prov->population >= 0)
        prov->population += 25;
    }
  }


  void SetProvinceOwner(entt::registry &registry, u32 owner, Vector2 clickPos)
  {
        i32 provId = DetermineTileIdFromClick(clickPos);
        assert(provId >= 0);

        auto provincesView = registry.view<Provinces>();
        auto provincesEntity = provincesView.front();
        Provinces &provinces = provincesView.get<Provinces>(provincesEntity);

        for (std::shared_ptr<Province> prov: provinces)
        {

          if (prov->id == (u32) provId)
          {
            prov->owner = owner;
            switch (prov->owner)
            {
              case 0:
                prov->name = "Rome";
                break;
              case 1:
                prov->name = "Athens";
                break;
              case 2:
                prov->name = "Lugudunon";
                break;
              case 3:
                prov->name = "Carthage";
                break;
              case 4:
                prov->name = "Persepolis";
                break;
            }
          }
        }
  }

  void DrawProvinces(entt::registry &reg, TextureCache &cache)
  {
    auto view = reg.view<Provinces>();
    Provinces &provinces = view.get<Provinces>(view.front());

    for (std::shared_ptr<Province> prov: provinces)
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

      //       str coordString = std::to_string((u32)tile.coord.x) + "," +
      //                         std::to_string((u32)tile.coord.y);
      //       const char *coordText = coordString.c_str();
      ////       if (.debug)
      //         DrawText(coordText, tile.position.x + 16.0, tile.position.y + 16.0, 14,
      //                  BLUE);

      // str popString = std::to_string(tile.population);
      // const char *text = popString.c_str();
      // if (debug)
      //   DrawText(text, tile.position.x + 16.0, tile.position.y + 32.0,
      //   14,
      //            RED);

      if (prov->owner > -1)
      {
        Color color = BLACK;

        Rectangle frameRec = {0.0, 0.0, 128, 128.0};

        switch (prov->owner)
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

        DrawTextureRec(cache.handle(hstr{"factionOverlay"})->texture,
                       frameRec, prov->tile->position, Fade(WHITE, 0.5));
      }

      if (prov->population >= 100)
      {
        // DrawSingleBorder(tile);
        DrawTextureV(cache.handle(hstr{"romanVillageTexture"})->texture,
                     prov->tile->position, WHITE);
        if (prov->name != "")
          DrawText(prov->name.c_str(), prov->tile->position.x + 50.0,
                   prov->tile->position.y + 86.0, 14, WHITE);
      }
    }
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
