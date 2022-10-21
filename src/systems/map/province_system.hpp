#include "../../common.hpp"
#include "../../components/province.hpp"
#include "../../components/settlement.hpp"
#include "../../renderer/textures.hpp"
#include "../event_system.hpp"
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

inline void SetProvinceOwner( entt::registry &reg, u32 owner );
inline void DrawProvinces( entt::registry &, TextureCache &, bool );

// TODO figure out if this really needs to be here
// Should be able to make it like the listener in ui_system.
struct ProvListener : EventSystem::Listener {
  inline void Receive() override {
    if ( this->currState == nullptr || this->currReg == nullptr ) {
      return;
    }

    printf( "ProvinceSystem got an event!\n" );
    SetProvinceOwner( *this->currReg, this->currState->currPlayer->id );
  }

  inline void Listen() {
    EventSystem::dispatcher.sink<Event::ProvEvent>()
      .connect<&ProvListener::Receive>( this );
  }
};


inline ProvListener listener;

inline void InitProvinces( entt::registry &reg, TextureCache &cache ) {
  auto tView = reg.view<Terrain::TileMap>();
  Terrain::TileMap tiles = tView.get<Terrain::TileMap>( tView.front() );


  for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
    entt::entity provEnt = reg.create();
    reg.emplace<Province::Component>( provEnt, i, -1, false, tiles[i] );
  }

  listener.Listen();
}

inline void UpdateProvinces( State &state, entt::registry &reg ) {
  listener.Update( state, reg );

  // auto view = reg.view<Province::Component>();

  // for ( auto &ent: view ) {
  //   Province::Component &prov = view.get<Province::Component>( ent );

  //   if ( prov.owner > -1 ) {
  //     Settlement::Component &settlement = reg.get<Settlement::Component>( ent );

  //     UpdateSettlement( settlement );
  //   }
  // }
}

inline void
AssignProvince( entt::registry &registry, u32 owner, Vector2 clickPos ) {
  i32 provId = DetermineTileIdFromClick( clickPos );
  assert( provId >= 0 );

  auto provinces = registry.view<Province::Component>();

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

inline void SetProvinceOwner( entt::registry &reg, u32 owner ) {
  auto selectedView = reg.view<Selected::Component, Unit::Component>();
  auto selectedEntity = selectedView.front();

  if ( selectedEntity == entt::null ) {
    printf( "No selected entity, cancelling\n" );
    return;
  }

  Unit::Component &unit = selectedView.get<Unit::Component>( selectedEntity );

  AssignProvince( reg, owner, unit.position );
}


inline void
DrawProvinces( entt::registry &reg, TextureCache &cache, bool showOverlays ) {
  auto provinces = reg.view<Province::Component>();
  auto settlements = reg.view<Settlement::Component>();

  for ( auto ent: provinces ) {
    Province::Component &prov = provinces.get<Province::Component>( ent );
    // Settlement::Component &settlement =
    //   settlements.get<Settlement::Component>( ent );

    // str idString = std::to_string(tile.id);
    // const char *idText = idString.c_str();
    // if (debug)
    //   DrawText(idText, tile.position.x + 16.0, tile.position.y + 16.0,
    //   14,
    //            WHITE);

    // str ownerString = std::to_string(tile.owner);
    // const char *ownertext = ownerString.c_str();
    // DrawText(ownertext, tile.position.x + 48.0, tile.position.y + 16.0,
    // 14,
    //          BLUE);

    //       str coordString = std::to_string((u32)tile.coord.x) + "," +
    //                         std::to_string((u32)tile.coord.y);
    //       const char *coordText = coordString.c_str();
    ////       if (.debug)
    //         DrawText(coordText, tile.position.x + 16.0, tile.position.y + 16.0, 14,
    //                  BLUE);

    // str popString = std::to_string(tile.population);
    // const char *text = popString.c_str();
    // if (debug)
    //   DrawText(text, tile.position.x + 16.0, tile.position.y + 32.0,
    //   14,
    //            RED);

    // if ( prov.owner <= -1 || settlement.population.current <= 0 )
    //   continue;


    // Vector2 settlement_pos = {
    //   prov.tile->position.x + 24,
    //   prov.tile->position.y + 24,
    // };

    // // DrawRectangleRec({provPos.x + 50,
    // //                   provPos.y + 86, 128, 64},
    // //                  Fade(WHITE, 0.8f));
    // // DrawSingleBorder(tile);

    // // Draw Settlement
    // DrawTextureV( settlement.texture, settlement_pos, WHITE );
  }
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
