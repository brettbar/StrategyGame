#pragma once

#include "../../shared/common.hpp"
#include "../../shared/fonts.hpp"
#include "../../shared/global.hpp"
#include "../../shared/textures.hpp"
#include "../components/faction.hpp"
#include "../components/player.hpp"
#include "../components/settlement.hpp"
#include "map_system.hpp"
#include "province_system.hpp"
#include <raylib.h>

namespace OverlaySystem {

  inline void draw_default() {
    auto provinces = Global::world.view<Province::Component>();

    for ( auto entity: provinces ) {
      auto &prov = provinces.get<Province::Component>( entity );

      if ( prov.tile->owner != entt::null ) {
        Player::Component player =
          Global::world.get<Player::Component>( prov.tile->owner );
        Faction::Component faction =
          Global::world.get<Faction::Component>( prov.tile->owner );

        Rectangle frameRec = { 0.0, 0.0, TILE_WIDTH, TILE_HEIGHT };

        DrawTextureRec(
          Global::texture_cache[hstr{ ( faction.primary_color + "Overlay" )
                                        .c_str() }]
            ->texture,
          frameRec,
          prov.tile->position,
          Fade( WHITE, 0.25 )
        );


        auto neighbors = MapSystem::get_neighbors( *prov.tile );

        // @leftoff, need to get these in the correct order
        Vector4 edge_coords[6] = {
          Vector4{ 0, 16, 32, 0 },
          Vector4{ 64, 16, 64, 48 },
          Vector4{ 32, 0, 64, 16 },
          Vector4{ 64, 48, 32, 64 },
          Vector4{ 32, 64, 0, 48 },
          Vector4{ 0, 48, 0, 16 },
        };

        for ( u32 i = 0; i < neighbors.size(); i++ ) {
          auto neighbor = neighbors[i];
          if ( neighbor->owner != prov.tile->owner )
            DrawLineEx(
              { prov.tile->position.x + edge_coords[i].x,
                prov.tile->position.y + edge_coords[i].y },
              { prov.tile->position.x + edge_coords[i].z,
                prov.tile->position.y + edge_coords[i].w },
              2,
              RED
            );
          break;
        }
      }
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

        DrawTextureRec(
          Global::texture_cache[hstr{ ( faction.primary_color + "Overlay" )
                                        .c_str() }]
            ->texture,
          frameRec,
          prov.tile->position,
          Fade( WHITE, 0.5 )
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
