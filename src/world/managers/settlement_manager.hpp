#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"

namespace Settlement {
  class IManager {
public:
    map<std::string, Image> building_map;

    static IManager *Get() {
      static IManager instance;
      return &instance;
    }
  
    void on_start() {
      Image buildings = LoadImageFromTexture(
        Global::texture_cache[hstr{ "buildings" }]->texture
      );

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

    void on_load() {
      on_start();
    }
  };


  static IManager *Manager() {
    static IManager instance;
    return &instance;
  }
};
