#pragma once

#include "../events.hpp"
#include "../renderer/textures.hpp"
#include "map_system.hpp"
#include "spawn_system.hpp"
#include <raylib.h>

namespace ProvinceSystem {

inline void SetProvinceOwner( u32 owner );

// TODO figure out if this really needs to be here
// Should be able to make it like the listener in ui_system.
// struct ProvListener : Events::Listener {
//   inline void Receive() override {
//     if ( this->currState == nullptr ) {
//       return;
//     }

//     printf( "ProvinceSystem got an event!\n" );
//     SetProvinceOwner( this->currState->currPlayer->id );
//   }

//   inline void Listen() {
//     Events::dispatcher.sink<Event::ProvEvent>().connect<&ProvListener::Receive>(
//       this );
//   }
// };


// inline ProvListener listener;

inline void Init( TextureCache &cache ) {
  auto tView = Global::world.view<MapSystem::TileMap>();
  MapSystem::TileMap tiles = tView.get<MapSystem::TileMap>( tView.front() );


  for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
    entt::entity entity = Global::world.create();

    Province::Component prov = {
      .id = i,
      .owner = entt::null,
      .selected = false,
      .tile = tiles[i],
      .resources = {},
    };

    switch ( prov.tile->biome ) {
      case Biome::Plains: {
        prov.resources.push_back( NaturalResource::Trees );
      } break;
    }

    Global::world.emplace<Province::Component>( entity, prov );
  }

  // listener.Listen();
}

inline void Draw( Camera2D &camera ) {
  Texture2D tex = Global::texture_cache[hstr{ "lumber.png" }]->texture;
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
        camera.target.y - ( camera.offset.y / camera.zoom ) - 32 ) {
      continue;
    }

    switch ( prov.tile->biome ) {
      case Biome::Plains: {
        // std::cout << "Foo!!!" << '\n';
        // std::cout << "Bar!!!" << '\n';
        DrawTextureV(
          tex,
          { prov.tile->position.x + 16.0f, prov.tile->position.y + 16.0f },
          WHITE );
        // std::cout << "Baz!!!" << '\n';
      } break;
    }
  }
}

inline void AssignProvince( entt::entity owner, Vector2 pos ) {
  i32 provId = DetermineTileIdFromPosition( pos );
  assert( provId >= 0 );

  auto provinces = Global::world.view<Province::Component>();

  for ( auto ent: provinces ) {
    auto &prov = provinces.get<Province::Component>( ent );

    if ( prov.id != (u32) provId || prov.tile->biome == Biome::Sea )
      continue;

    prov.owner = owner;
    // Settlement::Component settlement = {
    //   .id = prov.id,
    //   .population =
    //     {
    //       .current = 200,
    //       .birthRate = 40,
    //       .deathRate = 10,
    //       .growthRate = ( 40.0f - 10.0f ) / 200,
    //       .carryingCapacity = 1000,
    //     },
    // };

    // switch ( prov.owner ) {
    //   case 0: {
    //     settlement.name = "Rome";
    //     u32 choice = RollN( 4 );
    //     printf( "choice %d\n", choice );

    //     std::string foo = "roman_s" + std::to_string( choice );
    //     std::cout << "std::string " << foo << "\n";

    //     const char *bar = foo.c_str();
    //     printf( "c_str %s", bar );

    //     Image base = GenImageColor( 128, 128, ColorAlpha( WHITE, 0.0 ) );
    //     ImageAlphaPremultiply( &base );
    //     ImageDraw(
    //       &base,
    //       building_map.at( bar ),
    //       { 0, 0, 16, 16 },
    //       { 0, 0, 16, 16 },
    //       WHITE );
    //     settlement.texture = LoadTextureFromImage( base );
    //   } break;
    //   case 1:
    //     settlement.name = "Athens";
    //     break;
    //   case 2:
    //     settlement.name = "Lugudunon";
    //     break;
    //   case 3:
    //     settlement.name = "Carthage";
    //     break;
    //   case 4:
    //     settlement.name = "Persepolis";
    //     break;
    // }

    // registry.emplace<Settlement::Component>( ent, settlement );
  }
}

inline void SetProvinceOwner( entt::entity owner ) {
  auto selectedView =
    Global::world.view<Selected::Component, Unit::Component>();
  auto selectedEntity = selectedView.front();

  if ( selectedEntity == entt::null ) {
    printf( "No selected entity, cancelling\n" );
    return;
  }

  Unit::Component &unit = selectedView.get<Unit::Component>( selectedEntity );

  AssignProvince( owner, unit.position );
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
};// namespace ProvinceSystem
