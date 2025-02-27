#pragma once

#include "../../shared/textures.hpp"

#include "../managers/map_manager.hpp"
#include "resource_system.hpp"

namespace ProvinceSystem {

  inline void SetProvinceOwner( u32 owner );

  inline rect GetTileTextureRect( u32 texture_i ) {
    f32 offset = 2.0f;

    f32 atlas_slot_w = TILE_WIDTH + ( offset * 2 );
    f32 atlas_slot_h = TILE_HEIGHT + ( offset * 2 );

    // plains
    if ( texture_i >= 0 && texture_i <= 13 ) {
      return {
        offset + (atlas_slot_w) *texture_i, offset, TILE_WIDTH, TILE_HEIGHT
      };
    }
    // mtns
    else if ( texture_i == 14 ) {
      return { offset, offset + atlas_slot_h, TILE_WIDTH, TILE_HEIGHT };
    }
    // forrest
    else if ( texture_i == 15 ) {
      return { offset, offset + atlas_slot_h * 2, TILE_WIDTH, TILE_HEIGHT };
    }
    // sea
    else if ( texture_i == 16 ) {
      return { offset, offset + atlas_slot_h * 3, TILE_WIDTH, TILE_HEIGHT };
    }
    // hills
    else if ( texture_i == 17 ) {
      return { offset, offset + atlas_slot_h * 4, TILE_WIDTH, TILE_HEIGHT };
    }
    // desert
    else if ( texture_i == 18 ) {
      return { offset, offset + atlas_slot_h * 5, TILE_WIDTH, TILE_HEIGHT };
    }
    // tundra
    else if ( texture_i == 19 ) {
      return { offset, offset + atlas_slot_h * 6, TILE_WIDTH, TILE_HEIGHT };
    }
    // invalid
    else {
      // placeholder empty
      return { 2048.0f, 2048.0f, TILE_WIDTH, TILE_HEIGHT };
    }
  }

  inline void Init() {
    for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
      entt::entity prov_entity = Global::world.create();

      Province::Component prov = {
        .selected = false,
        .tile = Map::Manager()->tile_map[i],
        .resources = {},
      };

      ResourceSystem::SpawnResource( prov );

      Global::world.emplace<Province::Component>( prov_entity, prov );
    }
  }

  inline void DrawTileTerrain( Tile::Component tile ) {
    // Texture2D texture =
    //   Global::texture_cache[hstr{ tile.texture_key.c_str() }]->texture;
    // Rectangle frameRec = { 0.0f, 0.0f, TILE_WIDTH, TILE_HEIGHT };
    Texture2D texture = Global::texture_cache[hstr{ "terrain.png" }]->texture;
    // Rectangle frameRec = { 0.0f, 0.0f, TILE_WIDTH, TILE_HEIGHT };
    Rectangle frameRec = GetTileTextureRect( tile.texture_i );

    // Texture2D snow_tile = cache[hstr{ "snow_tile" }]->texture;
    // DrawTextureRec(hex, {frameRec.x + 520.0f, frameRec.y, frameRec.width, frameRec.height}, tile.position, WHITE);
    // DrawTextureRec(hex, frameRec, tile.position, WHITE);
    // DrawTextureRec( water_tile, frameRec, tile->position, WHITE );
    DrawTextureRec( texture, frameRec, tile.position, WHITE );
  }

  inline void Draw( Camera2D &camera ) {
    // Texture2D tex = Global::texture_cache[hstr{ "timber.png" }]->texture;

    auto view = Global::world.view<Province::Component>();

    // Draw backwards to make sure out-of-hex overlaps work
    for ( auto it = view.rbegin(), last = view.rend(); it != last; ++it ) {
      entt::entity entity = *it;

      auto &prov = Global::world.get<Province::Component>( entity );

      if ( prov.tile->position.x - TILE_WIDTH >
             camera.target.x + ( camera.offset.x / camera.zoom ) + 32 ||
           prov.tile->position.x + TILE_WIDTH <
             camera.target.x - ( camera.offset.x / camera.zoom ) - 32 ||
           prov.tile->position.y - TILE_WIDTH >
             camera.target.y + ( camera.offset.y / camera.zoom ) + 32 ||
           prov.tile->position.y + TILE_WIDTH <
             camera.target.y - ( camera.offset.y / camera.zoom ) - 32 ) {
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
      if ( neighbor->owner == entt::null &&
           Map::Manager()->biome_inhabitable( neighbor->biome ) )
        return std::make_shared<vec2f>( neighbor->position );
    }

    return nullptr;
  }

};// namespace ProvinceSystem
