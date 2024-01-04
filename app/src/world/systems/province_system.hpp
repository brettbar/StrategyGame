#pragma once

#include "../../shared/textures.hpp"
#include "../components/selected.hpp"
#include "map_system.hpp"
#include "resource_system.hpp"

namespace ProvinceSystem {

  inline void SetProvinceOwner( u32 owner );

  inline void Init() {
    Global::world.clear<Province::Component>();

    for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
      entt::entity prov_entity = Global::world.create();

      Province::Component prov = {
        .selected = false,
        .tile = MapSystem::Manager()->tile_map[i],
        .resources = {},
      };

      ResourceSystem::SpawnResource( prov );


      // switch ( prov.tile->biome )
      // {
      //   case Biome::Plains:
      //   {
      //     prov.resources.push_back( Resources::Natural::Trees );
      //   }
      //   break;
      // }

      Global::world.emplace<Province::Component>( prov_entity, prov );
    }
  }

  inline void DrawTileTerrain( Tile::Component tile ) {
    Texture2D texture =
      Global::texture_cache[hstr{ tile.texture_key.c_str() }]->texture;

    Rectangle frameRec = { 0.0f, 0.0f, TILE_WIDTH, TILE_HEIGHT };

    // Texture2D snow_tile = cache[hstr{ "snow_tile" }]->texture;
    // DrawTextureRec(hex, {frameRec.x + 520.0f, frameRec.y, frameRec.width, frameRec.height}, tile.position, WHITE);
    // DrawTextureRec(hex, frameRec, tile.position, WHITE);
    // DrawTextureRec( water_tile, frameRec, tile->position, WHITE );
    DrawTextureRec( texture, frameRec, tile.position, WHITE );
  }

  inline void Draw( Camera2D &camera ) {
    // Texture2D tex = Global::texture_cache[hstr{ "lumber.png" }]->texture;
    for ( auto entity: Global::world.view<Province::Component>() ) {

      auto &prov = Global::world.get<Province::Component>( entity );

      if (
      prov.tile->position.x - TILE_WIDTH >
        camera.target.x + ( camera.offset.x / camera.zoom ) + 32 ||
      prov.tile->position.x + TILE_WIDTH <
        camera.target.x - ( camera.offset.x / camera.zoom ) - 32 ||
      prov.tile->position.y - TILE_WIDTH >
        camera.target.y + ( camera.offset.y / camera.zoom ) + 32 ||
      prov.tile->position.y + TILE_WIDTH <
        camera.target.y - ( camera.offset.y / camera.zoom ) - 32 )
      {
        continue;
      }

      DrawTileTerrain( *prov.tile );
    }
  }

  inline bool player_has_province( entt::entity owner ) {
    auto settlements = Global::world.view<Province::Component>();

    for ( auto settlement_e: settlements ) {
      auto prov = settlements.get<Province::Component>( settlement_e );
      if ( prov.tile->owner == owner ) {
        return true;
      }
    }

    return false;
  }


  inline void AssignProvince( entt::entity owner, Vector2 pos ) {
    i32 prov_id = DetermineTileIdFromPosition( pos );
    assert( prov_id >= 0 );

    auto provinces = Global::world.view<Province::Component>();

    for ( auto entity: provinces ) {
      auto &prov = provinces.get<Province::Component>( entity );

      if ( prov.tile->id != (u32) prov_id || prov.tile->biome == Biome::Sea )
        continue;

      prov.tile->owner = owner;
    }
  }

  inline void claim_province_for_player(
    entt::entity owner,
    vec2f colonist_pos
  ) {
    AssignProvince( owner, colonist_pos );
  }

  inline void SetProvinceOwner( entt::entity owner ) {
    auto selectedView =
      Global::world.view<Selected::Component, Actor::Component>();
    auto selectedEntity = selectedView.front();

    if ( selectedEntity == entt::null ) {
      printf( "No selected entity, cancelling\n" );
      return;
    }

    Actor::Component &actor =
      selectedView.get<Actor::Component>( selectedEntity );

    AssignProvince( owner, actor.position );
  }
};// namespace ProvinceSystem
