#pragma once

#include "../../shared/global.hpp"
#include "../../shared/utils.hpp"

#include "../components/player_component.hpp"
#include "../components/province_component.hpp"
#include "../components/settlement_component.hpp"
#include "../components/stockpile_component.hpp"
#include <raylib.h>

namespace Resource {

struct System {
  static void init_stockpiles() {
    auto player_v = Global::world.view<Player::Component>();

    for (const auto &player_e: player_v) {

      auto stockpile_c = Stockpile::Component();

      stockpile_c.resources = {
        {Resources::Type::Timber, 10},
        {Resources::Type::Bricks, 10},
        {Resources::Type::Stone, 10},
        {Resources::Type::Fish, 10},
        {Resources::Type::Wheat, 10},
      };

      Global::world.emplace<Stockpile::Component>(player_e, stockpile_c);
    }
  }


  static void update_stockpiles() {
    auto settlement_v =
      Global::world.view<Province::Component, Settlement::Component>();
    auto stockpile_v = Global::world.view<Stockpile::Component>();

    for (const auto &settlement_e: settlement_v) {
      auto [province, settlement] = settlement_v.get(settlement_e);

      entt::entity owner_e = province.owner;

      if (owner_e == entt::null) {
        continue;
      }

      Stockpile::Component &stockpile =
        stockpile_v.get<Stockpile::Component>(owner_e);

      for (u32 i = 0; i < (u32) Resources::Type::COUNT; i++) {
        Resources::Type rt = (Resources::Type) i;

        u32 num_in_province = settlement.resource_quantities[rt];

        stockpile.resources[rt] += num_in_province;
      }
    }
  }

  // temp, probably want a better way to do this
  static map<Resources::Type, u32> get_resources_for_player(
    entt::entity player_e
  ) {
    auto stockpiles =
      Global::world.view<Stockpile::Component, Player::Component>();

    u32 total = 0;

    for (entt::entity stockpile_e: stockpiles) {
      auto [stockpile, player] = stockpiles.get(stockpile_e);

      if (player.id == player_e) {
        return stockpile.resources;
      }
    }

    return {};
  }


  static void update_1tps() {
    update_stockpiles();
  }

  static void SpawnResource(Province::Component &prov) {
    f32 randomFloat = random_u32_inclmax(0, 10);

    switch (prov.tile.biome) {
      case Biome::Mountains: {
        if (randomFloat > 8) {
          prov.natural_resources.push_back(Resources::Natural::IronVein);
        } else if (randomFloat > 7) {
          prov.natural_resources.push_back(Resources::Natural::TinVein);
        } else if (randomFloat > 6) {
          prov.natural_resources.push_back(Resources::Natural::CopperVein);
        }
      } break;
      case Biome::Hills:
        if (randomFloat > 7)
          prov.natural_resources.push_back(Resources::Natural::Clay);
        break;
      case Biome::Forest:
        prov.natural_resources.push_back(Resources::Natural::Trees);
        break;
      case Biome::Plains:
        break;
      case Biome::Desert:
        break;
      case Biome::Sea: {
        if (randomFloat > 7)
          prov.natural_resources.push_back(Resources::Natural::Fish);
      } break;
      default:
        break;
    }
  }


  static void DrawResource(Province::Component &prov) {
    if (prov.resources.size() == 0)
      return;

    for (auto resource: prov.natural_resources) {
      DrawCircle(
        prov.tile.top_left_corner_position.x + 32,
        prov.tile.top_left_corner_position.y + 32,
        16,
        BLACK
      );

      hstr id = {};
      switch (resource) {
        case Resources::Natural::CopperVein:
          id = "copper_ore.png";
          break;
        case Resources::Natural::TinVein:
          id = "tin_ore.png";
          break;
        case Resources::Natural::IronVein:
          id = "iron_ore.png";
          break;
        case Resources::Natural::Clay:
          id = "clay.png";
          break;
        case Resources::Natural::Trees:
          id = "timber.png";
          break;
        case Resources::Natural::Fish:
          id = "fish.png";
          break;
        default:
          return;
      }

      DrawTexture(
        Global::texture_cache[id]->texture,
        prov.tile.top_left_corner_position.x + 16,
        prov.tile.top_left_corner_position.y + 16,
        WHITE
      );
    }
  }

  static void DrawRawMaterial(
    Province::Component &prov,
    Settlement::Component &settlement
  ) {
    if (settlement.resource_quantities.size() == 0)
      return;

    DrawCircle(
      prov.tile.top_left_corner_position.x + 32,
      prov.tile.top_left_corner_position.y + 32,
      16,
      BLACK
    );

    for (auto resource_pair: settlement.resource_quantities) {
      Resources::Type resource = resource_pair.first;
      u32 quantity = resource_pair.second;

      if (quantity < 1) {
        continue;
      }


      hstr id = Resources::resource_icon_path(resource);

      DrawTexture(
        Global::texture_cache[id]->texture,
        prov.tile.top_left_corner_position.x + 16,
        prov.tile.top_left_corner_position.y + 16,
        WHITE
      );

      cstr quant = u32_to_cstr(quantity);
      DrawTextEx(
        Global::font_cache[hstr{"font_romulus"}]->font,
        quant,
        {
          prov.tile.top_left_corner_position.x + 36,
          prov.tile.top_left_corner_position.y + 36,
        },
        8,
        2,
        WHITE
      );
    }
  }

  static void Draw(Camera2D &camera) {
    // Texture2D tex = Global::texture_cache[hstr{ "lumber.png" }]->texture;

    auto view = Global::world.view<Province::Component>();
    for (auto entity: view) {

      auto &prov = view.get<Province::Component>(entity);

      if (prov.tile.top_left_corner_position.x - TILE_WIDTH >
            camera.target.x + (camera.offset.x / camera.zoom) + 32 ||
          prov.tile.top_left_corner_position.x + TILE_WIDTH <
            camera.target.x - (camera.offset.x / camera.zoom) - 32 ||
          prov.tile.top_left_corner_position.y - TILE_WIDTH >
            camera.target.y + (camera.offset.y / camera.zoom) + 32 ||
          prov.tile.top_left_corner_position.y + TILE_WIDTH <
            camera.target.y - (camera.offset.y / camera.zoom) - 32) {
        continue;
      }

      auto settlement = Global::world.try_get<Settlement::Component>(entity);

      if (settlement) {
        DrawRawMaterial(prov, *settlement);
      }

      DrawResource(prov);
    }
  }

  static void init() {
    init_stockpiles();
  }

  // struct Really {
  //   std::string resource;
  //   std::string value;

  //   template<class Archive>
  //   void serialize( Archive &ar ) {
  //     ar( CEREAL_NVP( resource ) );
  //     CEREAL_NVP( value );
  //   }
  // };
  // https://uscilab.github.io/cereal/serialization_archives.html
  // https://stackoverflow.com/questions/64865338/cereal-unable-to-deserialize-a-json-string-unhandled-exception
  // void epilogue( cereal::JSONInputArchive &, const Really & ) {}
  // void prologue( cereal::JSONInputArchive &, const Really & ) {}
  // static void LoadData() {
  //   std::ifstream file( "data/really.json" );
  //   cereal::JSONInputArchive input{ file };

  //   Really resources;
  //   input( resources );

  //   std::cout << "SUCCESS INPUT" << std::endl;
  //   cereal::JSONOutputArchive pretty_print( std::cout );
  //   pretty_print( resources );
  // }
};
};// namespace Resource
