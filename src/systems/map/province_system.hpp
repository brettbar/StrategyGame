#include "../../common.hpp"
#include "../../components/province.hpp"
#include "../../components/settlement.hpp"
#include "../../events.hpp"
#include "../../renderer/textures.hpp"
#include "../spawn_system.hpp"
#include "terrain_system.hpp"
#include <array>
#include <raylib.h>
#include <thread>
// TODO REMOVE
#include <iostream>
#include <string>

#pragma once

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
  auto tView = Global::registry.view<Terrain::TileMap>();
  Terrain::TileMap tiles = tView.get<Terrain::TileMap>( tView.front() );


  for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
    entt::entity provEnt = Global::registry.create();
    Global::registry
      .emplace<Province::Component>( provEnt, i, -1, false, tiles[i] );
  }

  // listener.Listen();
}

inline void Update( State &state ) {
  // listener.Update( state );

  // auto view = reg.view<Province::Component>();

  // for ( auto &ent: view ) {
  //   Province::Component &prov = view.get<Province::Component>( ent );

  //   if ( prov.owner > -1 ) {
  //     Settlement::Component &settlement = reg.get<Settlement::Component>( ent );

  //     UpdateSettlement( settlement );
  //   }
  // }
}

inline void AssignProvince( u32 owner, Vector2 pos ) {
  i32 provId = DetermineTileIdFromPosition( pos );
  assert( provId >= 0 );

  auto provinces = Global::registry.view<Province::Component>();

  for ( auto ent: provinces ) {
    Province::Component &prov = provinces.get<Province::Component>( ent );

    if ( prov.id != (u32) provId || prov.tile->biome == Tile::Biome::WATER )
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

inline void SetProvinceOwner( u32 owner ) {
  auto selectedView =
    Global::registry.view<Selected::Component, Unit::Component>();
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
