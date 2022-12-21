#pragma once

#include "../../shared/common.hpp"
#include "../../shared/fonts.hpp"
#include "../../shared/global.hpp"
#include "../../shared/textures.hpp"
#include "../components/settlement.hpp"
#include "province_system.hpp"
#include <raylib.h>

namespace OverlaySystem {

inline void DrawProvinceOverlays( TextureCache &texture_cache ) {
  auto provinces = Global::world.view<Province::Component>();

  for ( auto entity: provinces ) {
    auto &prov = provinces.get<Province::Component>( entity );

    if ( prov.owner != entt::null ) {
      Player::Component player =
        Global::world.get<Player::Component>( prov.owner );

      Rectangle frameRec = { 0.0, 0.0, TILE_WIDTH, TILE_HEIGHT };

      DrawTextureRec(
        texture_cache[hstr{ "redOverlay" }]->texture,
        frameRec,
        prov.tile->position,
        Fade( WHITE, 0.5 )
      );
      // switch ( player.faction ) {
      //   case Faction::ID::Romans:
      //     DrawTextureRec(
      //       texture_cache[hstr{ "redOverlay" }]->texture,
      //       frameRec,
      //       prov.tile->position,
      //       Fade( WHITE, 0.5 )
      //     );
      //     break;
      //   case Faction::ID::Greeks:
      //     DrawTextureRec(
      //       texture_cache[hstr{ "blueOverlay" }]->texture,
      //       frameRec,
      //       prov.tile->position,
      //       Fade( WHITE, 0.5 )
      //     );
      //     break;
      //   case Faction::ID::Celts:
      //     DrawTextureRec(
      //       texture_cache[hstr{ "greenOverlay" }]->texture,
      //       frameRec,
      //       prov.tile->position,
      //       Fade( WHITE, 0.5 )
      //     );
      //     break;
      //   case Faction::ID::Punics:
      //     DrawTextureRec(
      //       texture_cache[hstr{ "purpleOverlay" }]->texture,
      //       frameRec,
      //       prov.tile->position,
      //       Fade( WHITE, 0.5 )
      //     );
      //     break;
      //   case Faction::ID::Persians:
      //     DrawTextureRec(
      //       texture_cache[hstr{ "orangeOverlay" }]->texture,
      //       frameRec,
      //       prov.tile->position,
      //       Fade( WHITE, 0.5 )
      //     );
      //     break;
      //     // case Faction::ID::Germans:
      //     //   DrawTextureRec(
      //     //     texture_cache[hstr{ "greyOverlay" }]->texture,
      //     //     frameRec,
      //     //     prov.tile->position,
      //     //     Fade( WHITE, 0.5 ) );
      //     //   break;
      //     // case Faction::ID::Scythians:
      //     //   DrawTextureRec(
      //     //     texture_cache[hstr{ "pinkOverlay" }]->texture,
      //     //     frameRec,
      //     //     prov.tile->position,
      //     //     Fade( WHITE, 0.5 ) );
      //     //   break;
      //     break;
      // }
    }
  }
}

inline void DrawSettlementOverlays( TextureCache &texture_cache ) {
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

    const vec2 text_dims = MeasureTextEx(
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
