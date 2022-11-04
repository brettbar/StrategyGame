#pragma once

#include "../common.hpp"
#include "../components/province.hpp"
#include "../components/settlement.hpp"
#include "../global.hpp"
#include "../renderer/textures.hpp"
#include "selection_system.hpp"

namespace SettlementSystem {

inline void Init( TextureCache & );
inline void Update( State & );
inline void SpawnSettlement();
inline void UpdateSettlement( Settlement::Component & );
inline bool UpdatePopulation( Settlement::Component & );
inline void UpdateSprawl( Settlement::Component & );

inline void Draw( TextureCache &, bool );

inline std::map<std::string, Image> building_map;

inline void Init( TextureCache &cache ) {
  Image buildings = LoadImageFromTexture( cache[hstr{ "buildings" }]->texture );
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
}

inline void Update( State &state ) {
  auto view = Global::world.view<Province::Component, Settlement::Component>();

  for ( auto &ent: view ) {
    Province::Component &prov = view.get<Province::Component>( ent );

    if ( prov.owner != entt::null ) {
      Settlement::Component &settlement =
        Global::world.get<Settlement::Component>( ent );

      UpdateSettlement( settlement );
    }
  }
}

// inline Texture Temp() {
//         Image base = GenImageColor( 128, 128, ColorAlpha( WHITE, 0.0 ) );
//         // ImageAlphaPremultiply( &base );
//         ImageDraw(
//           &base,
//           building_map.at( "roman_m1" ),
//           { 0, 0, 16, 16 },
//           { 0, 0, 16, 16 },
//           WHITE );
//         return LoadTextureFromImage( base );
// }

inline void SpawnSettlement() {
  Unit::Component unit =
    Global::world.get<Unit::Component>( SelectionSystem::selected_entity );
  entt::entity unit_entity = SelectionSystem::selected_entity;

  vec2 pos = unit.position;
  i32 closest_tile = DetermineTileIdFromPosition( pos );

  if ( closest_tile == -1 )
    return;

  for ( auto entity: Global::world.view<Province::Component>() ) {
    Province::Component &prov =
      Global::world.get<Province::Component>( entity );

    // TODO pretty sure I am checking this twice, another time in the Actor colonist area
    if ( prov.tile->id == closest_tile ) {
      if ( prov.owner == unit.owner ) {
        if ( !Global::world.any_of<Settlement::Component>( entity ) ) {
          printf( "spawning settlement\n" );

          Settlement::Component settlement = {
            .id = prov.id,
            .name = "Rome",
            .development = Settlement::Development::Village,
            .population =
              {
                .current = 200,
                .birthRate = 40,
                .deathRate = 10,
                .growthRate = ( 40.0f - 10.0f ) / 200,
                .carryingCapacity = 1000,
              },
            .texture = LoadTextureFromImage( building_map.at( "roman_m1" ) ),
          };

          Global::world.emplace<Settlement::Component>( entity, settlement );

          SelectionSystem::selected_entity = entt::null;
          Global::world.destroy( unit_entity );
          return;
        }
      }
    }
  }
}

inline void UpdateSettlement( Settlement::Component &settlement ) {
  bool needs_sprawl_update = UpdatePopulation( settlement );

  // Update Sprawl
  if ( !needs_sprawl_update )
    return;

  printf( "Sprawl increase at %d\n", settlement.population.current );

  // TODO determine development from population
  settlement.development = Settlement::Development::Village;


  // Image base = LoadImageFromTexture( settlement.texture );


  // for ( u32 i = 0; i < ( settlement.population.current / 100 ); i++ ) {
  //   ImageDraw(
  //     &base,
  //     building_map.at( "roman_s3" ),
  //     { 0, 0, 16, 16 },
  //     { ( i * 8.0f ), 0, 16, 16 },
  //     WHITE );
  // }

  // TODO maybe expensive?
  settlement.texture = LoadTextureFromImage( building_map.at( "roman_m1" ) );
}

inline bool UpdatePopulation( Settlement::Component &settlement ) {
  Settlement::Population &pop = settlement.population;

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

inline void Draw( TextureCache &cache, bool showOverlays ) {
  auto settlements =
    Global::world.view<Settlement::Component, Province::Component>();

  for ( auto entity: settlements ) {
    Province::Component &province =
      settlements.get<Province::Component>( entity );
    Settlement::Component &settlement =
      settlements.get<Settlement::Component>( entity );

    // str idString = std::to_string(tile.id);
    // const char *idText = idString.c_str();
    // if (debug)
    //   DrawText(idText, tile.position.x + 16.0, tile.position.y + 16.0,
    //   14
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


    Vector2 settlement_pos = {
      province.tile->position.x + 24,
      province.tile->position.y + 24,
    };

    // // DrawRectangleRec({provPos.x + 50,
    // //                   provPos.y + 86, 128, 64},
    // //                  Fade(WHITE, 0.8f));
    // // DrawSingleBorder(tile);

    // // Draw Settlement
    DrawPerfectTexture( settlement.texture, settlement_pos, WHITE );
  }
}

};// namespace SettlementSystem
