#pragma once

#include "../renderer/textures.hpp"
#include "map_system.hpp"
#include "spawn_system.hpp"
#include <raylib.h>

namespace ProvinceSystem {

inline void SetProvinceOwner( u32 owner );

inline void Init() {
  // for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
  auto tile_view = Global::world.view<Tile::Component>();

  for ( entt::entity tile_entity: tile_view ) {


    auto &tile = Global::world.get<Tile::Component>( tile_entity );

    entt::entity prov_entity = Global::world.create();
    Province::Component prov = {
      .owner = entt::null,
      .selected = false,
      .resources = {},
    };

    switch ( tile.biome ) {
      case Biome::Plains: {
        prov.resources.push_back( NaturalResource::Trees );
      } break;
    }

    Global::world.emplace<Province::Component>( tile_entity, prov );
  }
}

inline void Draw( Camera2D &camera ) {
  Texture2D tex = Global::texture_cache[hstr{ "lumber.png" }]->texture;
  for ( auto entity:
        Global::world.view<Tile::Component, Province::Component>() ) {

    auto &tile = Global::world.get<Tile::Component>( entity );
    auto &prov = Global::world.get<Province::Component>( entity );

    if (
      tile.position.x - TILE_WIDTH >
        camera.target.x + ( camera.offset.x / camera.zoom ) + 32 ||
      tile.position.x + TILE_WIDTH <
        camera.target.x - ( camera.offset.x / camera.zoom ) - 32 ||
      tile.position.y - TILE_WIDTH >
        camera.target.y + ( camera.offset.y / camera.zoom ) + 32 ||
      tile.position.y + TILE_WIDTH <
        camera.target.y - ( camera.offset.y / camera.zoom ) - 32 ) {
      continue;
    }

    switch ( tile.biome ) {
      case Biome::Plains: {
        // std::cout << "Foo!!!" << '\n';
        // std::cout << "Bar!!!" << '\n';
        DrawTextureV(
          tex, { tile.position.x + 16.0f, tile.position.y + 16.0f }, WHITE
        );
        // std::cout << "Baz!!!" << '\n';
      } break;
    }
  }
}

inline void AssignProvince( entt::entity owner, Vector2 pos ) {
  i32 provId = DetermineTileIdFromPosition( pos );
  assert( provId >= 0 );

  auto provinces = Global::world.view<Tile::Component, Province::Component>();

  for ( auto entity: provinces ) {
    auto &tile = provinces.get<Tile::Component>( entity );
    auto &prov = provinces.get<Province::Component>( entity );

    if ( tile.id != (u32) provId || tile.biome == Biome::Sea )
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
};// namespace ProvinceSystem


//  Tile *FindTileByCoord(TileMap &tileMap, u32 x, u32 y)
//  {
//
//    if ((x >= 0 && x <= MAP_WIDTH) && (y >= 0 && y <= MAP_HEIGHT))
//    {
//      return tileMap.tiles[index(x, y)];
//    }
//    return nullptr;
//  }
