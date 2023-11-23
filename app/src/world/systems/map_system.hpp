#pragma once

#include "../../shared/global.hpp"
#include "../../shared/textures.hpp"
#include "../../shared/utils.hpp"
#include "../components/actor.hpp"
#include "../components/province.hpp"
#include "../components/settlement.hpp"
#include "../components/sight.hpp"
#include <algorithm>
#include <chrono>
#include <raylib.h>
#include <stdlib.h>
#include <string>

namespace MapSystem {

  enum class Mode {
    Terrain,
    Political,
  };

  inline Mode mode = Mode::Terrain;

  using NoiseMap = std::array<float, MAP_WIDTH * MAP_HEIGHT>;
  using TileMap =
    std::array<std::shared_ptr<Tile::Component>, MAP_WIDTH * MAP_HEIGHT>;

  inline TileMap tile_map = {};
  inline vec2f points[MAP_WIDTH * MAP_HEIGHT];

  inline void Init();
  inline void Draw( Camera2D &, TextureCache & );
  inline void UpdateFOW();
  inline NoiseMap GeneratePerlinNoise( float, int, float );
  inline void FilterIslands( NoiseMap &, f32 );
  inline NoiseMap NormalizeMap( NoiseMap );

  inline void dump_print_noise( NoiseMap noise ) {
    for ( u32 i = 0; i < MAP_HEIGHT; ++i ) {
      for ( u32 j = 0; j < MAP_WIDTH; ++j ) {
        printf( "%.2f ", noise[IndexFromCoords( i, j, MAP_WIDTH )] );
      }
      printf( "\n" );
    }
  }

  inline void assign_terrain( NoiseMap noise, f32 sea_lvl ) {
    for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
      u32 x = i % MAP_WIDTH;
      u32 y = i / MAP_HEIGHT;

      f32 xPos = x * TILE_WIDTH;
      f32 yPos = y * ( TILE_HEIGHT * 0.75 );

      Vector2 position;
      Biome biome;

      f32 noise_val = noise[IndexFromCoords( x, y, MAP_WIDTH )];

      if ( noise_val >= 0.75f )
        biome = Biome::Mountains;
      else if ( noise_val >= 0.7f )
        biome = Biome::Hills;
      // else if ( noise >= 0.7f )
      //   biome = Biome::Forest;
      else if ( noise_val >= sea_lvl )
        biome = Biome::Plains;
      else
        biome = Biome::Sea;

      if ( y % 2 == 1 )
        position = { xPos + ( TILE_WIDTH / 2 ), yPos };
      else
        position = { xPos, yPos };

      Tile::Component tile = {
        i,
        noise_val,
        position,
        {
          position.x + ( TILE_WIDTH / 2 ),
          position.y + ( TILE_HEIGHT / 2 ),
        },
        { x, y },
        biome,
        Tile::Visibility::UNEXPLORED,
      };

      tile_map[IndexFromCoords( x, y, MAP_WIDTH )] =
        std::make_shared<Tile::Component>( tile );
    }
  }

  inline f32 random_float( f32 min, f32 max ) {
    return min + ( (f32) rand() / RAND_MAX ) * ( max - min );
  }

  inline void print_points( const vec2f *points, int num_points ) {}

  // Voronoi
  inline void voronoi_init() {
    // 1. generate list of points
    f32 min_x = 0;
    f32 min_y = 0;
    f32 max_x = TILE_WIDTH * MAP_WIDTH;
    f32 max_y = TILE_HEIGHT * MAP_HEIGHT;

    for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
      points[i].x = random_float( min_x, max_x );
      points[i].y = random_float( min_y, max_y );
    }
  }

  inline void voronoi_draw() {}

  inline void Init() {

    voronoi_init();

    f32 seed = 25;
    NoiseMap pNoise = GeneratePerlinNoise( seed, 7, 1.2f );

    //    FilterIslands(pNoise, waterLevel);
    NormalizeMap( pNoise );

    assign_terrain( pNoise, 0.3 );
  }

  //  @attempt
  // inline void Init() {
  //   // 0. NOISE
  //   f32 seed = 25;
  //   NoiseMap noise = GeneratePerlinNoise( seed, 7, 1.2f );
  //   // NormalizeMap( noise );

  //   printf( "Perlin Noise\n" );

  //   NoiseMap crust = GeneratePerlinNoise( seed, 7, 1.2f );
  //   // NormalizeMap( crust );

  //   f32 noise_factor = 10;
  //   f32 crust_factor = 6;
  //   f32 tectonic_factor = 3;
  //   f32 pangaea = -5;

  //   // 1. TECTONICS
  //   {
  //     auto tectonic_median = approximateQuantile( crust, 0.5 );

  //     NoiseMap tectonic = {};

  //     for ( u32 i = 0; i < crust.size(); i++ ) {
  //       tectonic[i] = ( 0.2f / abs( tectonic_median - crust[i] ) + 0.1 ) - 0.95;
  //     }


  //     NoiseMap elevation = {};

  //     for ( u32 i = 0; i < noise.size(); i++ ) {
  //       elevation[i] =
  //         5 + noise[i] * noise_factor + crust[i] * crust_factor +
  //         tectonic[i] * tectonic_factor +
  //         -pangaea * ( abs( (f32) i / ( MAP_WIDTH * MAP_HEIGHT ) - 0.5 ) +
  //                      abs( (f32) ( i % MAP_WIDTH ) / MAP_WIDTH - 0.5 ) );
  //     }

  //     dump_print_noise( elevation );

  //     elevation = NormalizeMap( elevation );

  //     f32 sea_lvl = approximateQuantile( elevation, 0.2 );
  //     f32 flatness = 0.5f;

  //     NoiseMap final = {};

  //     for ( u32 i = 0; i < elevation.size(); i++ ) {
  //       if ( elevation[i] < sea_lvl ) {
  //         final[i] = -pow( 1 - elevation[i] / sea_lvl, 0.35 );
  //       } else {
  //         final[i] = pow(
  //           ( ( elevation[i] - sea_lvl ) * ( 0.5 + tectonic[i] * 0.5 ) ) /
  //             ( 1 - sea_lvl ),
  //           1 + 2 * flatness
  //         );
  //       }
  //     }

  //     assign_terrain( final, sea_lvl );
  //   }
  // }


  inline void Draw( Camera2D &camera, TextureCache &cache ) {
    Texture2D land_tile = cache[hstr{ "land_tile" }]->texture;
    Texture2D forest_tile = cache[hstr{ "forest_tile" }]->texture;
    Texture2D water_tile = cache[hstr{ "water_tile" }]->texture;
    Texture2D hills_tile = cache[hstr{ "hills_tile" }]->texture;
    Texture2D sand_tile = cache[hstr{ "sand_tile" }]->texture;
    Texture2D mountains_tile = cache[hstr{ "mountains_tile" }]->texture;
    // Texture2D snow_tile = cache[hstr{ "snow_tile" }]->texture;

    Rectangle frameRec = { 0.0f, 0.0f, TILE_WIDTH, TILE_HEIGHT };

    for ( auto &tile: tile_map ) {

      if (
      tile->position.x - TILE_WIDTH >
        camera.target.x + ( camera.offset.x / camera.zoom ) ||
      tile->position.x + TILE_WIDTH <
        camera.target.x - ( camera.offset.x / camera.zoom ) ||
      tile->position.y - TILE_WIDTH >
        camera.target.y + ( camera.offset.y / camera.zoom ) ||
      tile->position.y + TILE_WIDTH <
        camera.target.y - ( camera.offset.y / camera.zoom ) )
      {
        continue;
      }


      //        DrawTextureRec(hex, {frameRec.x + 520.0f, frameRec.y, frameRec.width, frameRec.height}, tile.position, WHITE);
      //    DrawTextureRec(hex, frameRec, tile.position, WHITE);
      switch ( tile->biome ) {
        case Biome::Mountains:
          // DrawTextureRec( snow_tile, frameRec, tile.position, WHITE );
          DrawTextureRec( mountains_tile, frameRec, tile->position, WHITE );
          break;
        case Biome::Hills:
          // DrawTextureRec( hills_tile, frameRec, tile->position, WHITE );
          DrawTextureRec( hills_tile, frameRec, tile->position, WHITE );
          break;
        case Biome::Forest:
          // DrawTextureRec( hills_tile, frameRec, tile->position, WHITE );
          DrawTextureRec( forest_tile, frameRec, tile->position, WHITE );
          break;
        case Biome::Plains:
          // DrawTextureRec( land_tile, frameRec, tile->position, WHITE );
          DrawTextureRec( land_tile, frameRec, tile->position, WHITE );
          break;
        case Biome::Desert:
          // DrawTextureRec( sand_tile, frameRec, tile->position, WHITE );
          DrawTextureRec( sand_tile, frameRec, tile->position, WHITE );
          break;
        case Biome::Sea:
          // DrawTextureRec( water_tile, frameRec, tile->position, WHITE );
          DrawTextureRec( water_tile, frameRec, tile->position, WHITE );
          break;
        default:
          // DrawTextureRec( water_tile, frameRec, tile->position, WHITE );
          DrawTextureRec( water_tile, frameRec, tile->position, WHITE );
          break;
      }

      // switch (tile->visibility)
      // {
      //   case VISIBILE:
      //     break;
      //   case EXPLORED:
      //     DrawTextureRec(hex, {frameRec.x + 260, frameRec.y, frameRec.width, frameRec.height}, tile->position, Fade(BLACK, 0.5f));
      //     break;
      //   case UNEXPLORED:
      //     DrawTextureRec(hex, {frameRec.x + 260f, frameRec.y, frameRec.width, frameRec.height}, tile->position, BLACK);
      //     break;
      //   default:
      //     break;
      // }
    }
    for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
      vec2f point = points[i];

      if (
      point.x - TILE_WIDTH >
        camera.target.x + ( camera.offset.x / camera.zoom ) ||
      point.x + TILE_WIDTH <
        camera.target.x - ( camera.offset.x / camera.zoom ) ||
      point.y - TILE_WIDTH >
        camera.target.y + ( camera.offset.y / camera.zoom ) ||
      point.y + TILE_WIDTH <
        camera.target.y - ( camera.offset.y / camera.zoom ) )
      {
        continue;
      }

      DrawCircle( point.x, point.y, 2, RED );
    }
  }

  inline void UpdateFOW() {
    auto view = Global::world.view<Actor::Component, Sight::Component>();

    for ( auto &entity: view ) {
      Sight::Component sight = view.get<Sight::Component>( entity );
      Actor::Component actor = view.get<Actor::Component>( entity );

      i32 closestId = DetermineTileIdFromPosition( actor.position );
      assert( closestId >= 0 );

      std::shared_ptr<Tile::Component> closest = tile_map[closestId];
      u32 x = closest->coords.x;
      u32 y = closest->coords.y;

      std::array<std::shared_ptr<Tile::Component>, 6> neighbors = {
        tile_map[IndexFromCoords( x, y - 1, MAP_WIDTH )],
        tile_map[IndexFromCoords( x + 1, y, MAP_WIDTH )],
        tile_map[IndexFromCoords( x, y + 1, MAP_WIDTH )],
        tile_map[IndexFromCoords( x - 1, y + 1, MAP_WIDTH )],
        tile_map[IndexFromCoords( x - 1, y, MAP_WIDTH )],
        tile_map[IndexFromCoords( x - 1, y - 1, MAP_WIDTH )],
      };

      if ( y % 2 == 1 ) {
        neighbors[0] = tile_map[IndexFromCoords( x + 1, y - 1, MAP_WIDTH )];
        neighbors[2] = tile_map[IndexFromCoords( x + 1, y + 1, MAP_WIDTH )];
        neighbors[3] = tile_map[IndexFromCoords( x, y + 1, MAP_WIDTH )];
        neighbors[5] = tile_map[IndexFromCoords( x, y - 1, MAP_WIDTH )];
      }

      for ( auto neighbor: neighbors ) {
        if ( neighbor != nullptr ) {

          neighbor->visibility = Tile::Visibility::VISIBILE;
        }
      }

      closest->visibility = Tile::Visibility::VISIBILE;
    }
  }

  // Inspired from code written by: OneLoneCoder Javidx9
  // https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_PerlinNoise.cpp
  inline NoiseMap GeneratePerlinNoise( float seed, int nOctaves, float fBias ) {
    srand( seed );
    //    srand(time(NULL));

    //    float *fSeed = new float[MAP_WIDTH * MAP_HEIGHT];

    NoiseMap fSeed = {};

    for ( int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ )
      fSeed[i] = (float) rand() / (float) RAND_MAX;

    // final array of perlin noise data
    NoiseMap fOutput;

    for ( int x = 0; x < MAP_WIDTH; x++ )
      for ( int y = 0; y < MAP_HEIGHT; y++ ) {
        // used to accumulate the perlin noise for a point, will be normalized in the end
        float fNoise = 0.0f;
        // used to sum all scaling factors, used to normalize our final perlin noise
        float fScaleAcc = 0.0f;
        // halved for each octave
        float fScale = 1.0f;

        for ( int o = 0; o < nOctaves; o++ ) {
          // We need two sample points to linearly interpolate between.
          // The distance between these sample points, is called pitch.

          // Each time we go up an octave, we halve the pitch.
          // If we know our dimensions are always a power of 2, they can be halved quite easily
          // Divides width by 2 o times
          int nPitch = MAP_WIDTH >> o;


          // The first sample is a multiple of the pitch.
          // Integer division sives out the remainder
          // So if x=20 and pitch=8, we know the sampleX1 should be at 16:
          // via (20 / 8) * 8 = 16
          int nSampleX1 = ( x / nPitch ) * nPitch;
          int nSampleY1 = ( y / nPitch ) * nPitch;

          // The second sample value is just the first sample value,
          // with the pitch added on.
          // Modulus lets us wrap around to get boundary coherence
          // (the first and last element lineup).
          int nSampleX2 = ( nSampleX1 + nPitch ) % MAP_WIDTH;
          int nSampleY2 = ( nSampleY1 + nPitch ) % MAP_WIDTH;

          // To perform the linear interpolation,
          // we need to know how far into a patricular pitch we are.
          //
          // We can find this by taking our current location,
          // minus our first sample point,
          // giving us a result between 0 and the pitch.
          //
          // If we divide by the pitch, we now have a normalized
          // value that tells us how far into the pitch we are.
          float fBlendX = (float) ( x - nSampleX1 ) / (float) nPitch;
          float fBlendY = (float) ( y - nSampleY1 ) / (float) nPitch;

          // Both these samples use the blendX parameter, this is because
          // We are effectively taking 2 slices of 1D perlin noise, and we will
          // use the blendY parameter to linearlly interpolate between the two samples.

          float fSampleT =
            ( 1.0f - fBlendX ) * fSeed[nSampleY1 * MAP_WIDTH + nSampleX1] +
            fBlendX * fSeed[nSampleY1 * MAP_WIDTH + nSampleX2];
          float fSampleB =
            ( 1.0f - fBlendX ) * fSeed[nSampleY2 * MAP_WIDTH + nSampleX1] +
            fBlendX * fSeed[nSampleY2 * MAP_WIDTH + nSampleX2];

          fScaleAcc += fScale;
          fNoise += ( fBlendY * ( fSampleB - fSampleT ) + fSampleT ) * fScale;
          fScale = fScale / fBias;
        }

        // set the normalized perlin noise to the element in the output array
        fOutput[y * MAP_WIDTH + x] = ( fNoise / fScaleAcc );
      }
    return fOutput;
  }

  inline void FilterIslands( NoiseMap &noiseMap, f32 waterLevel ) {
    for ( int x = 1; x < MAP_WIDTH - 1; x++ )
      for ( int y = 1; y < MAP_HEIGHT - 1; y++ ) {
        u32 numWater = 0;

        float NE = noiseMap.at( IndexFromCoords( x, y - 1, MAP_WIDTH ) );
        if ( NE < waterLevel )
          numWater++;

        float E = noiseMap.at( IndexFromCoords( x + 1, y, MAP_WIDTH ) );
        if ( E < waterLevel )
          numWater++;

        float SE = noiseMap.at( IndexFromCoords( x, y + 1, MAP_WIDTH ) );
        if ( SE < waterLevel )
          numWater++;

        float SW = noiseMap.at( IndexFromCoords( x - 1, y + 1, MAP_WIDTH ) );
        if ( SW < waterLevel )
          numWater++;

        float W = noiseMap.at( IndexFromCoords( x - 1, y, MAP_WIDTH ) );
        if ( W < waterLevel )
          numWater++;

        float NW = noiseMap.at( IndexFromCoords( x - 1, y - 1, MAP_WIDTH ) );
        if ( NW < waterLevel )
          numWater++;

        if ( numWater >= 5 ) {
          noiseMap[IndexFromCoords( x, y, MAP_WIDTH )] = 0.0f;
        }
      }
  }

  inline NoiseMap NormalizeMap( NoiseMap noise ) {
    NoiseMap normalized = {};

    f32 min = noise[0];
    f32 max = noise[0];

    for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
      if ( noise[i] < min )
        min = noise[i];
      if ( noise[i] > max )
        max = noise[i];
    }

    f32 range = max - min;

    for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
      normalized[i] = ( noise[i] - min ) / ( range );
    }

    return normalized;
  }
};// namespace MapSystem
