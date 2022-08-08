#pragma once

#include "../../common.hpp"
#include "../../renderer/fonts.hpp"
#include "../../renderer/textures.hpp"
#include "../map/province_system.hpp"
#include <raylib.h>

namespace OverlaySystem {

inline void Draw(
  entt::registry &reg,
  TextureCache &texture_cache,
  FontCache &font_cache ) {
  auto provinces = reg.view<Province::Component>();

  for ( auto ent: provinces ) {
    Province::Component &prov = provinces.get<Province::Component>( ent );

    if (
      prov.owner > -1 && prov.settlement != nullptr &&
      prov.settlement->population.current > 0 ) {
      Rectangle frameRec = { 0.0, 0.0, TILE_WIDTH, TILE_HEIGHT };

      switch ( prov.owner ) {
        case 0:
          DrawTextureRec(
            texture_cache[hstr{ "redOverlay" }]->texture,
            frameRec,
            prov.tile->position,
            Fade( WHITE, 0.5 ) );
          break;
        case 1:
          DrawTextureRec(
            texture_cache[hstr{ "blueOverlay" }]->texture,
            frameRec,
            prov.tile->position,
            Fade( WHITE, 0.5 ) );
          break;
        case 2:
          DrawTextureRec(
            texture_cache[hstr{ "greenOverlay" }]->texture,
            frameRec,
            prov.tile->position,
            Fade( WHITE, 0.5 ) );
          break;
        case 3:
          DrawTextureRec(
            texture_cache[hstr{ "purpleOverlay" }]->texture,
            frameRec,
            prov.tile->position,
            Fade( WHITE, 0.5 ) );
          break;
        case 4:
          DrawTextureRec(
            texture_cache[hstr{ "orangeOverlay" }]->texture,
            frameRec,
            prov.tile->position,
            Fade( WHITE, 0.5 ) );
          break;
      }

      // std::string popStr = "Pop: " + std::to_string(prov.settlement.population.current);

      // DrawText(popStr.c_str(), provPos.x + 50, provPos.y + 100, 10, BLACK);


      // DrawRectangleRec({provPos.x + 50, provPos.y + 86, 128, 14}, BLACK);

      // NOTE: I changed this from "" to nullptr
      if ( prov.settlement->name == nullptr )
        continue;

      Vector2 settlement_pos = {
        prov.tile->position.x + 48,
        prov.tile->position.y + 48,
      };

      DrawRectangleRec(
        {
          settlement_pos.x + 16,
          settlement_pos.y + 32,
          64,
          14,
        },
        BLACK );

      DrawTextEx(
        font_cache[hstr{ "font_romulus" }]->font,
        prov.settlement->name,
        {
          settlement_pos.x + 16.0f,
          settlement_pos.y + 32.0f,
        },
        14,
        2,
        WHITE );

      // DrawText(
      //   std::to_string( prov.settlement->population.current ).c_str(),
      //   settlement_pos.x + 54.0,
      //   settlement_pos.y + 32.0,
      //   14,
      //   WHITE );
    }
  }
}

}// namespace OverlaySystem
