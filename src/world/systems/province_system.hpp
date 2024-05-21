#pragma once

#include "../../shared/textures.hpp"

#include "../managers/map_manager.hpp"
#include "resource_system.hpp"

namespace ProvinceSystem {

  inline void SetProvinceOwner( u32 owner );

  inline void Init() {
    Global::world.clear<Province::Component>();

    for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
      entt::entity prov_entity = Global::world.create();

      Province::Component prov = {
        .selected = false,
        .tile = Map::Manager()->tile_map[i],
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
    auto provinces = Global::world.view<Province::Component>();

    for ( auto province_e: provinces ) {
      auto prov = provinces.get<Province::Component>( province_e );
      if ( prov.tile->owner == owner ) {
        return true;
      }
    }

    return false;
  }


  inline void colonist_claim_province( entt::entity colonist_e ) {
    Actor::Component &actor = Global::world.get<Actor::Component>( colonist_e );

    i32 prov_id = DetermineTileIdFromPosition( actor.position );
    assert( prov_id >= 0 );

    auto provinces = Global::world.view<Province::Component>();

    for ( auto entity: provinces ) {
      auto &prov = provinces.get<Province::Component>( entity );

      if ( prov.tile->id != (u32) prov_id || prov.tile->biome == Biome::Sea )
        continue;

      prov.tile->owner = actor.owner;
    }
  }

  inline static Province::Component *get_prov_from_vec2f( vec2f tile_pos ) {
    auto provinces = Global::world.view<Province::Component>();
    u32 tile_index = DetermineTileIdFromPosition( tile_pos );

    for ( auto prov_e: provinces ) {
      auto &prov = provinces.get<Province::Component>( prov_e );
      if ( prov.tile->id == tile_index ) {
        return &prov;
      }
    }

    return nullptr;
  }

  inline static sptr<vec2f> get_nearest_inhabitable_province( vec2f entity_pos
  ) {
    auto prov = get_prov_from_vec2f( entity_pos );
    if ( !prov ) {
      return nullptr;
    }

    auto neighbors = Map::Manager()->get_neighbors( *prov->tile );

    for ( auto neighbor: neighbors ) {
      if ( neighbor->owner == entt::null && Map::Manager()->biome_inhabitable( neighbor->biome ) )
        return std::make_shared<vec2f>( neighbor->position );
    }

    return nullptr;
  }

};// namespace ProvinceSystem
