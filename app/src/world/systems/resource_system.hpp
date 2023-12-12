#pragma once

#include "../../shared/global.hpp"
#include "../../shared/utils.hpp"
#include "../components/province.hpp"

namespace ResourceSystem {

  inline void SpawnResource( Province::Component &prov ) {
    f32 randomFloat = random_f32( 0, 10 );
    if ( randomFloat > 3 ) {
      return;
    }

    switch ( prov.tile->biome ) {
      case Biome::Mountains:
        if ( randomFloat > 2 ) {
          prov.resources.push_back( Resources::Natural::CopperVein );
        } else if ( randomFloat > 1 ) {
          prov.resources.push_back( Resources::Natural::TinVein );
        } else if ( randomFloat > 0 ) {
          prov.resources.push_back( Resources::Natural::IronVein );
        }
        break;
      case Biome::Hills:
        prov.resources.push_back( Resources::Natural::Clay );
        break;
      case Biome::Forest:
        prov.resources.push_back( Resources::Natural::Trees );
        break;
      case Biome::Plains:
        break;
      case Biome::Desert:
        break;
      case Biome::Sea:
        prov.resources.push_back( Resources::Natural::Fish );
        break;
      default:
        break;
    }
  }


  inline void DrawResource( Province::Component &prov ) {
    if ( prov.resources.size() == 0 )
      return;

    for ( auto resource: prov.resources ) {
      DrawCircle(
        prov.tile->position.x + 32, prov.tile->position.y + 32, 16, BLACK
      );

      hstr id = {};
      switch ( resource ) {
        case Resources::Natural::CopperVein:
          id = "copper_ore.png";
          break;
        case Resources::Natural::TinVein:
          id = "tin_ore.png";
          break;
        case Resources::Natural::IronVein:
          id = "iron_ore.png";
          break;
        case Resources::Natural::Clay:
          id = "clay.png";
          break;
        case Resources::Natural::Trees:
          id = "lumber.png";
          break;
        case Resources::Natural::Fish:
          id = "fish.png";
          break;
        default:
          return;
      }

      DrawTexture(
        Global::texture_cache[id]->texture,
        prov.tile->position.x + 16,
        prov.tile->position.y + 16,
        WHITE
      );
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

      DrawResource( prov );
    }
  }

  // struct Really {
  //   std::string resource;
  //   std::string value;

  //   template<class Archive>
  //   void serialize( Archive &ar ) {
  //     ar( CEREAL_NVP( resource ) );
  //     CEREAL_NVP( value );
  //   }
  // };
  // https://uscilab.github.io/cereal/serialization_archives.html
  // https://stackoverflow.com/questions/64865338/cereal-unable-to-deserialize-a-json-string-unhandled-exception
  // void epilogue( cereal::JSONInputArchive &, const Really & ) {}
  // void prologue( cereal::JSONInputArchive &, const Really & ) {}
  // inline void LoadData() {
  //   std::ifstream file( "data/really.json" );
  //   cereal::JSONInputArchive input{ file };

  //   Really resources;
  //   input( resources );

  //   std::cout << "SUCCESS INPUT" << std::endl;
  //   cereal::JSONOutputArchive pretty_print( std::cout );
  //   pretty_print( resources );
  // }

};// namespace ResourceSystem
