//
// Created by brett on 2/3/2022.
//

#pragma once

#include "../../shared/global.hpp"
#include "../../shared/textures.hpp"
#include "../../shared/utils.hpp"
#include "../components/actor.hpp"
#include "../components/province.hpp"
#include "../components/settlement.hpp"
#include "../components/sight.hpp"

// #include "../../common.hpp"
// #include "../../components/sight.hpp"
// #include "../../components/tile.hpp"
// #include "../../components/actor.hpp"
// #include "../../global.hpp"
// #include "../../renderer/textures.hpp"

#include <chrono>
#include <string>

namespace MapSystem {

  enum class Mode {
    Default,
    Terrain,
    Political,
    Resources,
  };

  inline Mode mode = Mode::Default;


  using NoiseMap = std::array<float, MAP_WIDTH * MAP_HEIGHT>;
  using TileMap =
    std::array<std::shared_ptr<Tile::Component>, MAP_WIDTH * MAP_HEIGHT>;

  inline TileMap tile_map = {};

  inline void Init();
  inline void UpdateFOW();
  inline u32 index( u32, u32 );
  inline NoiseMap GeneratePerlinNoise( float, int, float );
  inline void FilterIslands( NoiseMap &, f32 );
  inline void NormalizeMap( NoiseMap & );

  inline u32 index( u32 x, u32 y ) {
    return IndexFromCoords( x, y, MAP_WIDTH );
  }

  inline void Init() {
    f32 seed = 25;
    NoiseMap pNoise = GeneratePerlinNoise( seed, 7, 1.2f );

    float waterLevel = 0.59;
    //    FilterIslands(pNoise, waterLevel);
    NormalizeMap( pNoise );

    for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
      u32 x = i % MAP_WIDTH;
      u32 y = i / MAP_HEIGHT;

      f32 xPos = x * TILE_WIDTH;
      f32 yPos = y * ( TILE_HEIGHT * 0.75 );

      Vector2 position;
      Biome biome;

      f32 noise = pNoise[IndexFromCoords( x, y, MAP_WIDTH )];

      if ( noise >= 0.75f )
        biome = Biome::Mountains;
      else if ( noise >= 0.7f )
        biome = Biome::Hills;
      // else if ( noise >= 0.7f )
      //   biome = Biome::Forest;
      else if ( noise >= waterLevel )
        biome = Biome::Plains;
      else
        biome = Biome::Sea;

      if ( y % 2 == 1 )
        position = { xPos + ( TILE_WIDTH / 2 ), yPos };
      else
        position = { xPos, yPos };

      Tile::Component tile = {
        i,
        noise,
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
        tile_map[index( x, y - 1 )],
        tile_map[index( x + 1, y )],
        tile_map[index( x, y + 1 )],
        tile_map[index( x - 1, y + 1 )],
        tile_map[index( x - 1, y )],
        tile_map[index( x - 1, y - 1 )],
      };

      if ( y % 2 == 1 ) {
        neighbors[0] = tile_map[index( x + 1, y - 1 )];
        neighbors[2] = tile_map[index( x + 1, y + 1 )];
        neighbors[3] = tile_map[index( x, y + 1 )];
        neighbors[5] = tile_map[index( x, y - 1 )];
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

        float NE = noiseMap.at( index( x, y - 1 ) );
        if ( NE < waterLevel )
          numWater++;

        float E = noiseMap.at( index( x + 1, y ) );
        if ( E < waterLevel )
          numWater++;

        float SE = noiseMap.at( index( x, y + 1 ) );
        if ( SE < waterLevel )
          numWater++;

        float SW = noiseMap.at( index( x - 1, y + 1 ) );
        if ( SW < waterLevel )
          numWater++;

        float W = noiseMap.at( index( x - 1, y ) );
        if ( W < waterLevel )
          numWater++;

        float NW = noiseMap.at( index( x - 1, y - 1 ) );
        if ( NW < waterLevel )
          numWater++;

        if ( numWater >= 5 ) {
          noiseMap[index( x, y )] = 0.0f;
        }
      }
  }

  inline void NormalizeMap( NoiseMap &pNoise ) {
    float min = std::numeric_limits<float>::max();
    float max = 0;

    for ( int x = 0; x < MAP_WIDTH * MAP_HEIGHT; x++ ) {
      if ( pNoise[x] < min )
        min = pNoise[x];
      if ( pNoise[x] > max )
        max = pNoise[x];
    }

    for ( int x = 0; x < MAP_WIDTH * MAP_HEIGHT; x++ ) {
      pNoise[x] = ( pNoise[x] - min ) / ( max - min );
    }
  }


};// namespace MapSystem
