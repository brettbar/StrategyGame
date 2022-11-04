#pragma once

#include "../common.hpp"
#include "../components/settlement.hpp"
#include "../global.hpp"
#include "../renderer/fonts.hpp"
#include "../renderer/textures.hpp"
#include "province_system.hpp"
#include <raylib.h>

namespace OverlaySystem {

inline void DrawProvinceOverlays( TextureCache &texture_cache ) {
  auto provinces = Global::world.view<Province::Component>();

  for ( auto ent: provinces ) {
    Province::Component &prov = provinces.get<Province::Component>( ent );

    // if ( prov.owner > -1 ) {
    //   Rectangle frameRec = { 0.0, 0.0, TILE_WIDTH, TILE_HEIGHT };

    //   switch ( prov.owner ) {
    //     case 0:
    //       DrawTextureRec(
    //         texture_cache[hstr{ "redOverlay" }]->texture,
    //         frameRec,
    //         prov.tile->position,
    //         Fade( WHITE, 0.5 ) );
    //       break;
    //     case 1:
    //       DrawTextureRec(
    //         texture_cache[hstr{ "blueOverlay" }]->texture,
    //         frameRec,
    //         prov.tile->position,
    //         Fade( WHITE, 0.5 ) );
    //       break;
    //     case 2:
    //       DrawTextureRec(
    //         texture_cache[hstr{ "greenOverlay" }]->texture,
    //         frameRec,
    //         prov.tile->position,
    //         Fade( WHITE, 0.5 ) );
    //       break;
    //     case 3:
    //       DrawTextureRec(
    //         texture_cache[hstr{ "purpleOverlay" }]->texture,
    //         frameRec,
    //         prov.tile->position,
    //         Fade( WHITE, 0.5 ) );
    //       break;
    //     case 4:
    //       DrawTextureRec(
    //         texture_cache[hstr{ "orangeOverlay" }]->texture,
    //         frameRec,
    //         prov.tile->position,
    //         Fade( WHITE, 0.5 ) );
    //       break;
    //   }
    // }
  }
}

inline void DrawSettlementOverlays( TextureCache &texture_cache ) {
  auto settlements = Global::world.view<Settlement::Component>();

  for ( auto entity: settlements ) {
    Settlement::Component &settlement =
      settlements.get<Settlement::Component>( entity );
    Province::Component &prov =
      Global::world.get<Province::Component>( entity );

    // std::string popStr =
    //   "Pop: " + std::to_string( settlement.population.current );

    // DrawText( popStr.c_str(), provPos.x + 50, provPos.y + 100, 10, BLACK );


    // DrawRectangleRec( { provPos.x + 50, provPos.y + 86, 128, 14 }, BLACK );

    // NOTE: I changed this from "" to nullptr
    if ( settlement.name == "" )
      continue;

    Vector2 settlement_pos = {
      prov.tile->position.x + 24,
      prov.tile->position.y + 24,
    };

    const vec2 text_dims = MeasureTextEx(
      Global::font_cache[hstr{ "font_romulus" }]->font,
      settlement.name.c_str(),
      14,
      2.0f );


    DrawRectangleRec(
      {
        settlement_pos.x + 8,
        settlement_pos.y + 16,
        text_dims.x,
        text_dims.y,
      },
      BLACK );

    DrawTextEx(
      Global::font_cache[hstr{ "font_romulus" }]->font,
      settlement.name.c_str(),
      {
        settlement_pos.x + 8,
        settlement_pos.y + 16,
      },
      14,
      2,
      WHITE );

    // DrawText(
    //   std::to_string( settlement.population.current ).c_str(),
    //   settlement_pos.x + 54.0,
    //   settlement_pos.y + 32.0,
    //   14,
    //   WHITE );
  }
}

}// namespace OverlaySystem
