#include "../../common.hpp"
#include "../../components/province.hpp"
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
inline bool UpdatePopulation( Province::Settlement & );
inline void UpdateSettlement( Province::Settlement & );
inline void UpdateSprawl( Province::Settlement & );

inline std::map<std::string, Image> building_map;


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


  for ( u32 i = 0; i < Terrain::MAP_WIDTH * Terrain::MAP_HEIGHT; i++ ) {
    entt::entity provEnt = reg.create();

    reg.emplace<Province::Component>( provEnt, i, -1, nullptr, tiles[i] );
  }

  Image buildings =
    LoadImageFromTexture( cache.handle( hstr{ "buildings" } )->texture );
  Image roman_m1 = ImageFromImage( buildings, { 0, 0, 16, 16 } );
  Image roman_m2 = ImageFromImage( buildings, { 0, 16, 16, 16 } );
  Image roman_m3 = ImageFromImage( buildings, { 0, 32, 16, 16 } );
  Image roman_m4 = ImageFromImage( buildings, { 0, 48, 16, 16 } );

  Image roman_s1 = ImageFromImage( buildings, { 16, 0, 16, 16 } );
  Image roman_s2 = ImageFromImage( buildings, { 32, 0, 16, 16 } );
  Image roman_s3 = ImageFromImage( buildings, { 48, 0, 16, 16 } );
  Image roman_s4 = ImageFromImage( buildings, { 64, 0, 16, 16 } );

  building_map.insert_or_assign( "roman_m1", ( roman_m1 ) );
  building_map.insert_or_assign( "roman_m2", ( roman_m2 ) );
  building_map.insert_or_assign( "roman_m3", ( roman_m3 ) );
  building_map.insert_or_assign( "roman_m4", ( roman_m4 ) );

  building_map.insert_or_assign( "roman_s1", ( roman_s1 ) );
  building_map.insert_or_assign( "roman_s2", ( roman_s2 ) );
  building_map.insert_or_assign( "roman_s3", ( roman_s3 ) );
  building_map.insert_or_assign( "roman_s4", ( roman_s4 ) );


  listener.Listen();
}


inline void UpdateProvinces( State &state, entt::registry &reg ) {
  listener.Update( state, reg );

  auto view = reg.view<Province::Component>();

  for ( auto &ent: view ) {
    Province::Component &prov = view.get<Province::Component>( ent );

    if ( prov.owner > -1 && prov.settlement != nullptr )
      UpdateSettlement( *prov.settlement );
  }
}

inline void UpdateSettlement( Province::Settlement &settlement ) {
  bool needs_sprawl_update = UpdatePopulation( settlement );


  // Update Sprawl
  if ( !needs_sprawl_update )
    return;

  printf( "Sprawl increase at %d\n", settlement.population.current );

  // Image base = GenImageColor( 128, 128, ColorAlpha( WHITE, 0.0 ) );

  Image base = LoadImageFromTexture( settlement.texture );


  // ImageDraw( &base, image, { 0, 0, 16, 16 }, { 0, 0, 16, 16 }, WHITE );

  for ( u32 i = 0; i < ( settlement.population.current / 100 ); i++ ) {
    ImageDraw(
      &base,
      building_map.at( "roman_s3" ),
      { 0, 0, 16, 16 },
      { ( i * 8.0f ), 0, 16, 16 },
      WHITE );
  }

  settlement.texture = LoadTextureFromImage( base );
}

inline bool UpdatePopulation( Province::Settlement &settlement ) {
  Province::Population &pop = settlement.population;

  int before = settlement.population.current / 100;

  // if growing exponentially
  // P(t) = P0*e^(kt)

  // dP/dt = r * P ( 1 - P/K )
  // dP/dt = r * P ( K - P ) / K
  pop.growthRate = ( pop.birthRate - pop.deathRate ) / pop.current;
  f32 dP_over_dt = pop.growthRate * pop.current *
                   ( pop.carryingCapacity - pop.current ) /
                   pop.carryingCapacity;

  settlement.population.current += (i32) dP_over_dt;


  // // Find total number of digits - 1
  // int digits = (int) log10( before );

  // // Find first digit
  // before = (int) ( before / pow( 10, digits ) );

  // digits = (int) log10( settlement.population.current );
  // int after = (int) ( settlement.population.current / pow( 10, digits ) );
  int after = settlement.population.current / 100;

  if ( before < after ) {
    return true;
  }

  return false;
}

inline void
SpawnProvince( entt::registry &registry, u32 owner, Vector2 clickPos ) {
  i32 provId = DetermineTileIdFromClick( clickPos );
  assert( provId >= 0 );

  auto provinces = registry.view<Province::Component>();

  for ( auto ent: provinces ) {
    Province::Component &prov = provinces.get<Province::Component>( ent );

    if (
      prov.id != (u32) provId || prov.tile->biome == Tile::WATER ||
      prov.settlement != nullptr )
      continue;

    prov.owner = owner;

    prov.settlement = std::make_unique<Province::Settlement>();
    prov.settlement->id = prov.id;
    prov.settlement->population = {
      .current = 200,
      .birthRate = 40,
      .deathRate = 10,
      .growthRate = ( 40.0f - 10.0f ) / 200,
      .carryingCapacity = 1000,
    };

    switch ( prov.owner ) {
      case 0: {
        prov.settlement->name = "Rome";
        u32 choice = RollN( 4 );
        printf( "choice %d\n", choice );

        std::string foo = "roman_s" + std::to_string( choice );
        std::cout << "std::string " << foo << "\n";

        const char *bar = foo.c_str();
        printf( "c_str %s", bar );

        Image base = GenImageColor( 128, 128, ColorAlpha( WHITE, 0.0 ) );
        ImageDraw(
          &base,
          building_map.at( bar ),
          { 0, 0, 16, 16 },
          { 0, 0, 16, 16 },
          WHITE );
        prov.settlement->texture = LoadTextureFromImage( base );
      } break;
      case 1:
        prov.settlement->name = "Athens";
        break;
      case 2:
        prov.settlement->name = "Lugudunon";
        break;
      case 3:
        prov.settlement->name = "Carthage";
        break;
      case 4:
        prov.settlement->name = "Persepolis";
        break;
    }
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

  SpawnProvince( reg, owner, unit.position );
}


inline void
DrawProvinces( entt::registry &reg, TextureCache &cache, bool showOverlays ) {
  auto provinces = reg.view<Province::Component>();

  for ( auto ent: provinces ) {
    Province::Component &prov = provinces.get<Province::Component>( ent );
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

    if (
      prov.owner <= -1 || prov.settlement == nullptr ||
      prov.settlement->population.current <= 0 )
      continue;

    Color color = BLACK;

    Rectangle frameRec = { 0.0, 0.0, 128, 128.0 };

    switch ( prov.owner ) {
      case 0:
        color = RED;
        frameRec.x = 0.0f;
        break;
      case 1:
        color = SKYBLUE;
        frameRec.x = 128.0f;
        break;
      case 2:
        color = GREEN;
        frameRec.x = 256.0f;
        break;
      case 3:
        color = PURPLE;
        frameRec.x = 384.0f;
        break;
      case 4:
        color = ORANGE;
        frameRec.x = 513.0f;
        break;
    }

    // if ( showOverlays )
    //   DrawTextureRec(
    //     cache.handle( hstr{ "factionOverlay" } )->texture,
    //     frameRec,
    //     prov.tile->position,
    //     Fade( WHITE, 0.5 ) );


    Vector2 settlement_pos = {
      prov.tile->position.x + 48,
      prov.tile->position.y + 48,
    };

    // DrawRectangleRec({provPos.x + 50,
    //                   provPos.y + 86, 128, 64},
    //                  Fade(WHITE, 0.8f));
    // DrawSingleBorder(tile);

    // Draw Settlement
    DrawTextureV( prov.settlement->texture, settlement_pos, WHITE );


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
