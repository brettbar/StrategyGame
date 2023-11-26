#pragma once

#include "../../shared/textures.hpp"
#include "../components/selected.hpp"
#include "map_system.hpp"

namespace ProvinceSystem {

  inline void SetProvinceOwner( u32 owner );

  inline void Init() {
    for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
      entt::entity prov_entity = Global::world.create();

      Province::Component prov = {
        .selected = false,
        .tile = MapSystem::tile_map[i],
        .resources = {},
      };

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
    Texture2D land_tile = Global::texture_cache[hstr{ "land_tile" }]->texture;
    Texture2D forest_tile =
      Global::texture_cache[hstr{ "forest_tile" }]->texture;
    Texture2D water_tile = Global::texture_cache[hstr{ "water_tile" }]->texture;
    Texture2D hills_tile = Global::texture_cache[hstr{ "hills_tile" }]->texture;
    Texture2D sand_tile = Global::texture_cache[hstr{ "sand_tile" }]->texture;
    Texture2D mountains_tile =
      Global::texture_cache[hstr{ "mountains_tile" }]->texture;

    Rectangle frameRec = { 0.0f, 0.0f, TILE_WIDTH, TILE_HEIGHT };

    // Texture2D snow_tile = cache[hstr{ "snow_tile" }]->texture;
    // DrawTextureRec(hex, {frameRec.x + 520.0f, frameRec.y, frameRec.width, frameRec.height}, tile.position, WHITE);
    // DrawTextureRec(hex, frameRec, tile.position, WHITE);
    switch ( tile.biome ) {
      case Biome::Mountains:
        // DrawTextureRec( snow_tile, frameRec, tile.position, WHITE );
        DrawTextureRec( mountains_tile, frameRec, tile.position, WHITE );
        break;
      case Biome::Hills:
        // DrawTextureRec( hills_tile, frameRec, tile->position, WHITE );
        DrawTextureRec( hills_tile, frameRec, tile.position, WHITE );
        break;
      case Biome::Forest:
        // DrawTextureRec( hills_tile, frameRec, tile->position, WHITE );
        DrawTextureRec( forest_tile, frameRec, tile.position, WHITE );
        break;
      case Biome::Plains:
        // DrawTextureRec( land_tile, frameRec, tile->position, WHITE );
        DrawTextureRec( land_tile, frameRec, tile.position, WHITE );
        break;
      case Biome::Desert:
        // DrawTextureRec( sand_tile, frameRec, tile->position, WHITE );
        DrawTextureRec( sand_tile, frameRec, tile.position, WHITE );
        break;
      case Biome::Sea:
        // DrawTextureRec( water_tile, frameRec, tile->position, WHITE );
        DrawTextureRec( water_tile, frameRec, tile.position, WHITE );
        break;
      default:
        // DrawTextureRec( water_tile, frameRec, tile->position, WHITE );
        DrawTextureRec( water_tile, frameRec, tile.position, WHITE );
        break;
    }
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


  inline void AssignProvince( entt::entity owner, Vector2 pos ) {
    i32 prov_id = DetermineTileIdFromPosition( pos );
    assert( prov_id >= 0 );

    auto provinces = Global::world.view<Province::Component>();

    for ( auto entity: provinces ) {
      auto &prov = provinces.get<Province::Component>( entity );

      if ( prov.tile->id != (u32) prov_id || prov.tile->biome == Biome::Sea )
        continue;

      prov.tile->owner = owner;
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
      Global::world.view<Selected::Component, Actor::Component>();
    auto selectedEntity = selectedView.front();

    if ( selectedEntity == entt::null ) {
      printf( "No selected entity, cancelling\n" );
      return;
    }

    Actor::Component &actor =
      selectedView.get<Actor::Component>( selectedEntity );

    AssignProvince( owner, actor.position );
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
