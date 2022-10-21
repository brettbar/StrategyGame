//
// Created by brett on 12/29/2021.
//
#pragma once

#include "../../common.hpp"
#include "../../components/sight.hpp"
#include "../../components/tile.hpp"
#include "../../components/unit.hpp"
#include "../../global.hpp"
#include "../../renderer/textures.hpp"
#include "rlgl.h"
#include "terrain_system.hpp"
#include <chrono>
#include <string>

namespace Terrain {

using NoiseMap = std::array<float, MAP_WIDTH * MAP_HEIGHT>;
using TileMap =
  std::array<std::shared_ptr<Tile::Component>, MAP_WIDTH * MAP_HEIGHT>;

inline void Init();
inline void Draw( Camera2D &, TextureCache & );
inline void UpdateFOW();
inline u32 index( u32, u32 );
inline NoiseMap GeneratePerlinNoise( float, int, float );
inline void FilterIslands( NoiseMap &, f32 );
inline void NormalizeMap( NoiseMap & );

inline void Init() {
  entt::entity entity = Global::registry.create();
  TileMap tileMap = {};
  f32 seed = 25;
  NoiseMap pNoise = GeneratePerlinNoise( seed, 7, 1.2f );

  float waterLevel = 0.59;
  //    FilterIslands(pNoise, waterLevel);
  NormalizeMap( pNoise );

  for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
    // for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
    std::shared_ptr<Tile::Component> tile = std::make_shared<Tile::Component>();
    // tile.id = x + y * 128;
    tile->id = i;

    u32 x = i % MAP_WIDTH;
    u32 y = i / MAP_HEIGHT;

    f32 xPos = x * TILE_WIDTH;
    f32 yPos = y * ( TILE_HEIGHT * 0.75 );

    if ( y % 2 == 1 )
      tile->position = { xPos + ( TILE_WIDTH / 2 ), yPos };
    else
      tile->position = { xPos, yPos };

    tile->noise = pNoise[index( x, y )];

    // Montanas
    if ( tile->noise >= 0.8f )
      tile->biome = Tile::Biome::MTNS;
    // Colina
    else if ( tile->noise >= 0.75f )
      tile->biome = Tile::Biome::HILLS;
    // Tierra
    else if ( tile->noise >= waterLevel )
      tile->biome = Tile::Biome::LAND;
    //      // Playa
    //      else if (tile->noise >= 0.76f)
    //        tile->biome = BEACH;
    // Mar
    else
      tile->biome = Tile::Biome::WATER;

    tile->center = {
      tile->position.x + ( TILE_WIDTH / 2 ),
      tile->position.y + ( TILE_HEIGHT / 2 ) };
    tile->coords = { x, y };
    tile->visibility = Tile::Visibility::UNEXPLORED;
    tileMap[index( x, y )] = tile;
  }
  Global::registry.emplace<TileMap>( entity, tileMap );
}


inline void Draw( Camera2D &camera, TextureCache &cache ) {
  auto tilesView = Global::registry.view<TileMap>();
  auto tilesEntity = tilesView.front();
  TileMap &tileMap = tilesView.get<TileMap>( tilesEntity );

  Texture2D land_tile = cache[hstr{ "land_tile" }]->texture;
  Texture2D water_tile = cache[hstr{ "water_tile" }]->texture;
  Texture2D hills_tile = cache[hstr{ "hills_tile" }]->texture;
  Texture2D sand_tile = cache[hstr{ "sand_tile" }]->texture;
  Texture2D snow_tile = cache[hstr{ "snow_tile" }]->texture;

  Rectangle frameRec = { 0.0f, 0.0f, TILE_WIDTH, TILE_HEIGHT };

  for ( std::shared_ptr<Tile::Component> tile: tileMap ) {

    if (
      tile->position.x - TILE_WIDTH >
        camera.target.x + ( camera.offset.x / camera.zoom ) ||
      tile->position.x + TILE_WIDTH <
        camera.target.x - ( camera.offset.x / camera.zoom ) ||
      tile->position.y - TILE_WIDTH >
        camera.target.y + ( camera.offset.y / camera.zoom ) ||
      tile->position.y + TILE_WIDTH <
        camera.target.y - ( camera.offset.y / camera.zoom ) ) {
      continue;
    }


    //        DrawTextureRec(hex, {frameRec.x + 520.0f, frameRec.y, frameRec.width, frameRec.height}, tile->position, WHITE);
    //    DrawTextureRec(hex, frameRec, tile->position, WHITE);
    switch ( tile->biome ) {
      case Tile::Biome::MTNS:
        // DrawTextureRec( snow_tile, frameRec, tile->position, WHITE );
        DrawPerfectTexture( snow_tile, frameRec, tile->position, WHITE );
        break;
      case Tile::Biome::HILLS:
        // DrawTextureRec( hills_tile, frameRec, tile->position, WHITE );
        DrawPerfectTexture( hills_tile, frameRec, tile->position, WHITE );
        break;
      case Tile::Biome::LAND:
        // DrawTextureRec( land_tile, frameRec, tile->position, WHITE );
        DrawPerfectTexture( land_tile, frameRec, tile->position, WHITE );
        break;
      case Tile::Biome::BEACH:
        // DrawTextureRec( sand_tile, frameRec, tile->position, WHITE );
        DrawPerfectTexture( sand_tile, frameRec, tile->position, WHITE );
        break;
      case Tile::Biome::WATER:
        // DrawTextureRec( water_tile, frameRec, tile->position, WHITE );
        DrawPerfectTexture( water_tile, frameRec, tile->position, WHITE );
        break;
      default:
        // DrawTextureRec( water_tile, frameRec, tile->position, WHITE );
        DrawPerfectTexture( water_tile, frameRec, tile->position, WHITE );
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
}

inline void UpdateFOW() {
  auto view = Global::registry.view<Unit::Component, Sight::Component>();
  auto tileView = Global::registry.view<TileMap>();
  TileMap tiles = tileView.get<TileMap>( tileView.front() );

  for ( auto &entity: view ) {
    Sight::Component sight = view.get<Sight::Component>( entity );
    Unit::Component unit = view.get<Unit::Component>( entity );

    i32 closestId = DetermineTileIdFromPosition( unit.position );
    assert( closestId >= 0 );

    std::shared_ptr<Tile::Component> closest = tiles[closestId];
    u32 x = closest->coords.x;
    u32 y = closest->coords.y;

    std::array<std::shared_ptr<Tile::Component>, 6> neighbors = {
      tiles[index( x, y - 1 )],
      tiles[index( x + 1, y )],
      tiles[index( x, y + 1 )],
      tiles[index( x - 1, y + 1 )],
      tiles[index( x - 1, y )],
      tiles[index( x - 1, y - 1 )],
    };

    if ( y % 2 == 1 ) {
      neighbors[0] = tiles[index( x + 1, y - 1 )];
      neighbors[2] = tiles[index( x + 1, y + 1 )];
      neighbors[3] = tiles[index( x, y + 1 )];
      neighbors[5] = tiles[index( x, y - 1 )];
    }

    for ( auto &neighbor: neighbors ) {
      if ( neighbor != nullptr )
        neighbor->visibility = Tile::Visibility::VISIBILE;
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


// i = x + W * y;
inline u32 index( u32 x, u32 y ) { return x + MAP_WIDTH * y; };
};// namespace Terrain
