#include "../common.hpp"
#include "../components/settlement.hpp"
#include "../renderer/textures.hpp"

namespace SettlementSystem {

inline void UpdateSettlement( Settlement::Component & );
inline bool UpdatePopulation( Settlement::Component & );
inline void UpdateSprawl( Settlement::Component & );

inline std::map<std::string, Image> building_map;

inline void InitSettlements( TextureCache &cache ) {
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


inline void UpdateSettlement( Settlement::Component &settlement ) {
  bool needs_sprawl_update = UpdatePopulation( settlement );

  // Update Sprawl
  if ( !needs_sprawl_update )
    return;

  printf( "Sprawl increase at %d\n", settlement.population.current );

  // TODO determine development from population
  settlement.development = Settlement::Development::Village;

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

};// namespace SettlementSystem
