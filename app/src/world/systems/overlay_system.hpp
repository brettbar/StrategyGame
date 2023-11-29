#pragma once

#include "../../shared/common.hpp"
#include "../../shared/fonts.hpp"
#include "../../shared/global.hpp"
#include "../../shared/textures.hpp"
#include "../components/faction.hpp"
#include "../components/player.hpp"
#include "../components/settlement.hpp"
#include "faction_system.hpp"
#include "map_system.hpp"
#include "province_system.hpp"
#include <raylib.h>

namespace OverlaySystem {

  inline void draw_borders() {
    auto provinces = Global::world.view<Province::Component>();

    for ( auto entity: provinces ) {
      auto &prov = provinces.get<Province::Component>( entity );

      if ( prov.tile->owner == entt::null )
        continue;

      Player::Component player =
        Global::world.get<Player::Component>( prov.tile->owner );
      Faction::Component faction =
        Global::world.get<Faction::Component>( prov.tile->owner );

      auto neighbors = MapSystem::get_neighbors( *prov.tile );

      hstr borders[6] = {
        hstr{ ( "ne_border_" + faction.primary_color ).c_str() },
        hstr{ ( "e_border_" + faction.primary_color ).c_str() },
        hstr{ ( "se_border_" + faction.primary_color ).c_str() },
        hstr{ ( "sw_border_" + faction.primary_color ).c_str() },
        hstr{ ( "w_border_" + faction.primary_color ).c_str() },
        hstr{ ( "nw_border_" + faction.primary_color ).c_str() },
      };

      for ( u32 i = 0; i < neighbors.size(); i++ ) {
        auto neighbor = neighbors[i];
        if ( !neighbor )
          continue;

        if ( neighbor->owner != prov.tile->owner ) {
          DrawTexture(
            Global::texture_cache[borders[i]]->texture,
            prov.tile->position.x,
            prov.tile->position.y,
            WHITE
          );
        }
      }
    }
  }

  inline void draw_default() {
    auto provinces = Global::world.view<Province::Component>();

    for ( auto entity: provinces ) {
      auto &prov = provinces.get<Province::Component>( entity );

      if ( prov.tile->owner == entt::null )
        continue;

      Player::Component player =
        Global::world.get<Player::Component>( prov.tile->owner );
      Faction::Component faction =
        Global::world.get<Faction::Component>( prov.tile->owner );

      Rectangle frameRec = { 0.0, 0.0, TILE_WIDTH, TILE_HEIGHT };

      Color color = FactionSystem::color_map[faction.primary_color];

      DrawTextureRec(
        Global::texture_cache[hstr{ ( faction.primary_color + "_overlay" )
                                      .c_str() }]
          ->texture,
        frameRec,
        prov.tile->position,
        Fade( color, 0.1 )
      );
    }
  }

  inline void draw_political() {
    auto provinces = Global::world.view<Province::Component>();

    for ( auto entity: provinces ) {
      auto &prov = provinces.get<Province::Component>( entity );

      if ( prov.tile->owner != entt::null ) {
        Player::Component player =
          Global::world.get<Player::Component>( prov.tile->owner );
        Faction::Component faction =
          Global::world.get<Faction::Component>( prov.tile->owner );

        Rectangle frameRec = { 0.0, 0.0, TILE_WIDTH, TILE_HEIGHT };

        Color color = FactionSystem::color_map[faction.primary_color];

        DrawTextureRec(
          Global::texture_cache[hstr{ ( faction.primary_color + "_overlay" )
                                        .c_str() }]
            ->texture,
          frameRec,
          prov.tile->position,
          Fade( color, 0.5 )
        );
      }
    }
  }

  inline void draw_settlement_name() {
    auto settlements =
      Global::world.view<Province::Component, Settlement::Component>();

    for ( auto entity: settlements ) {
      auto &province = settlements.get<Province::Component>( entity );
      auto &settlement = settlements.get<Settlement::Component>( entity );

      // std::string popStr =
      //   "Pop: " + std::to_string( settlement.population.current );

      // DrawText( popStr.c_str(), provPos.x + 50, provPos.y + 100, 10, BLACK );


      // DrawRectangleRec( { provPos.x + 50, provPos.y + 86, 128, 14 }, BLACK );

      // NOTE: I changed this from "" to nullptr
      if ( settlement.name == "" )
        continue;

      Vector2 settlement_pos = {
        province.tile->position.x + 24,
        province.tile->position.y + 24,
      };

      const vec2f text_dims = MeasureTextEx(
        Global::font_cache[hstr{ "font_romulus" }]->font,
        settlement.name.c_str(),
        14,
        2.0f
      );


      DrawRectangleRec(
        {
          settlement_pos.x + 8,
          settlement_pos.y + 16,
          text_dims.x,
          text_dims.y,
        },
        BLACK
      );

      DrawTextEx(
        Global::font_cache[hstr{ "font_romulus" }]->font,
        settlement.name.c_str(),
        {
          settlement_pos.x + 8,
          settlement_pos.y + 16,
        },
        14,
        2,
        WHITE
      );

      // DrawText(
      //   std::to_string( settlement.population.current ).c_str(),
      //   settlement_pos.x + 54.0,
      //   settlement_pos.y + 32.0,
      //   14,
      //   WHITE );
    }
  }

}// namespace OverlaySystem
