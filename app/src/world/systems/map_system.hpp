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

#include <chrono>
#include <string>

struct MapSystem {
  using NoiseMap = std::array<f32, MAP_WIDTH * MAP_HEIGHT>;
  using TileMap = std::array<sptr<Tile::Component>, MAP_WIDTH * MAP_HEIGHT>;

  private:
  MapSystem() {}
  ~MapSystem() {}
  MapSystem( MapSystem const & ) = delete;
  void operator=( const MapSystem & ) = delete;

  public:
  enum class Mode {
    Default,
    Terrain,
    Political,
    Resources,
  };
  Mode mode = Mode::Default;


  f32 waterLevel = 0.3;
  f32 mtnsLevel = 0.7;
  f32 hillsLevel = 0.5;

  TileMap tile_map = {};
  u32 seed = 132;
  u32 octaves = 7;
  f32 scaling_bias = 1.2f;

  static MapSystem *Manager() {
    static MapSystem instance;
    return &instance;
  }

  void Init() {
    tile_map = {};

    NoiseMap pNoise = GeneratePerlinNoise( seed, octaves, scaling_bias );
    // NoiseMap pNoise = GeneratePerlinNoise( seed, 7, 0.8f );

    std::cout << "MapSystem::Init with wl: " << waterLevel << "\n";

    gen_islands( pNoise );
    //    FilterIslands(pNoise, waterLevel);

    for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
      u32 x = i % MAP_WIDTH;
      u32 y = i / MAP_HEIGHT;

      f32 xPos = x * TILE_WIDTH;
      f32 yPos = y * ( TILE_HEIGHT * 0.75 );

      Vector2 position;

      f32 noise = pNoise[IndexFromCoords( x, y, MAP_WIDTH )];

      Biome biome = determine_biome( noise );

      if ( y % 2 == 1 )
        position = { xPos + ( TILE_WIDTH / 2 ), yPos };
      else
        position = { xPos, yPos };

      Tile::Component tile = {
        .owner = entt::null,
        .id = i,
        .noise = noise,
        .position = position,
        .center =
          {
            position.x + ( TILE_WIDTH / 2 ),
            position.y + ( TILE_HEIGHT / 2 ),
          },
        .coords = { x, y },
        .biome = biome,
        .visibility = Tile::Visibility::UNEXPLORED,
        .texture_key = random_assign_sprite_from_biome( biome ),
      };

      tile_map[index( x, y )] = std::make_shared<Tile::Component>( tile );
    }
  }

  u32 index( u32 x, u32 y ) {
    if ( x < 0 || x > MAP_WIDTH || y < 0 || y > MAP_HEIGHT ) {
      return -1;
    }

    return IndexFromCoords( x, y, MAP_WIDTH );
  }

  f32 get_noise( vec2u coords ) {
    u32 i = index( coords.x, coords.y );

    if ( i < 0 || i >= tile_map.size() ) {
      return 0.0;
    }

    return tile_map[i]->noise;
  }

  void apply_radial_gradient( NoiseMap &noise, urect dims ) {
    u32 width = dims.width();
    // u32 height = dims.height();

    for ( u32 i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++ ) {
      vec2u coords = CoordsFromIndex( i, MAP_WIDTH );

      if (
        coords.x < dims.x || 
        coords.x > dims.z || 
        coords.y < dims.y || 
        coords.y > dims.w
      )
        continue;

      vec2u center = dims.center();

      f32 distance_x = ( center.x - coords.x ) * ( center.x - coords.x );
      f32 distance_y = ( center.y - coords.y ) * ( center.y - coords.y );
      f32 dist_to_center = sqrt( distance_x + distance_y );


      // extends how far out the radial is applied, lower means longer
      f32 scaling_factor = 0.8;

      dist_to_center = ( dist_to_center / width ) * scaling_factor;

      noise[i] = noise[i] - dist_to_center;
    }
  }

  // @todo fix this shit
  // @volatile right now i totally hardcode how many of each terrain sprites there are
  str random_assign_sprite_from_biome( Biome biome ) {
    switch ( biome ) {
      case Biome::Desert:
        return "sand1.bmp";
      case Biome::Forest: {
        u32 r = random_u32_inclmax( 1, 10 );
        return "forest" + std::to_string( r ) + ".bmp";
      }
      case Biome::Hills: {
        //@temp
        // return "hills1.bmp";
        u32 r = random_u32_inclmax( 1, 10 );
        return "forest" + std::to_string( r ) + ".bmp";
      }
      case Biome::Mountains:
        return "mountains1.bmp";
      case Biome::Plains: {
        u32 r = random_u32_inclmax( 1, 14 );
        return "grass" + std::to_string( r ) + ".bmp";
      }
      case Biome::Sea:
        return "water1.bmp";
      case Biome::Steppe:
        return "hills1.bmp";
      case Biome::Taiga:
      case Biome::Tundra:
        return "snow1.bmp";
    }
  }

  Biome determine_biome( f32 elevation ) {
    if ( elevation >= mtnsLevel )
      return Biome::Mountains;
    else if ( elevation >= hillsLevel )
      return Biome::Hills;
    // else if ( noise >= 0.7f )
    //   biome = Biome::Forest;
    else if ( elevation >= waterLevel )
      return Biome::Plains;
    else
      return Biome::Sea;
  }

  void gen_islands( NoiseMap &noise ) {

    // @todo this is likely wrong and has off-by-ones
    urect top_left = { 0, 0, 63, 63 };
    urect top_right = { 64, 0, 127, 63 };
    urect bottom_left = { 0, 64, 63, 127 };
    urect bottom_right = { 64, 64, 127, 127 };

    apply_radial_gradient( noise, top_left );

    apply_radial_gradient( noise, top_right );

    apply_radial_gradient( noise, bottom_left );

    apply_radial_gradient( noise, bottom_right );
  }


  std::array<sptr<Tile::Component>, 6> get_neighbors( Tile::Component tile ) {
    u32 x = tile.coords.x;
    u32 y = tile.coords.y;
    sptr<Tile::Component> ne_tile = nullptr;
    sptr<Tile::Component> e_tile = nullptr;
    sptr<Tile::Component> se_tile = nullptr;
    sptr<Tile::Component> sw_tile = nullptr;
    sptr<Tile::Component> w_tile = nullptr;
    sptr<Tile::Component> nw_tile = nullptr;

    if ( y % 2 == 0 ) {
      i32 ne = index( x, y - 1 );
      i32 e = index( x + 1, y );
      i32 se = index( x, y + 1 );
      i32 sw = index( x - 1, y + 1 );
      i32 w = index( x - 1, y );
      i32 nw = index( x - 1, y - 1 );

      if ( ne >= 0 )
        ne_tile = tile_map[ne];
      if ( e >= 0 )
        e_tile = tile_map[e];
      if ( se >= 0 )
        se_tile = tile_map[se];
      if ( sw >= 0 )
        sw_tile = tile_map[sw];
      if ( w >= 0 )
        w_tile = tile_map[w];
      if ( nw >= 0 )
        nw_tile = tile_map[nw];
    } else {
      i32 ne = index( x + 1, y - 1 );
      i32 e = index( x + 1, y );
      i32 se = index( x + 1, y + 1 );
      i32 sw = index( x, y + 1 );
      i32 w = index( x - 1, y );
      i32 nw = index( x, y - 1 );

      if ( ne >= 0 )
        ne_tile = tile_map[ne];
      if ( e >= 0 )
        e_tile = tile_map[e];
      if ( se >= 0 )
        se_tile = tile_map[se];
      if ( sw >= 0 )
        sw_tile = tile_map[sw];
      if ( w >= 0 )
        w_tile = tile_map[w];
      if ( nw >= 0 )
        nw_tile = tile_map[nw];
    }


    return std::array<sptr<Tile::Component>, 6>{
      ne_tile, e_tile, se_tile, sw_tile, w_tile, nw_tile };
  }


  void UpdateFOW() {
    auto view = Global::world.view<Actor::Component, Sight::Component>();

    for ( auto &entity: view ) {
      // Sight::Component sight = view.get<Sight::Component>( entity );
      Actor::Component actor = view.get<Actor::Component>( entity );

      i32 closestId = DetermineTileIdFromPosition( actor.position );
      assert( closestId >= 0 );

      std::shared_ptr<Tile::Component> closest = tile_map[closestId];
      u32 x = closest->coords.x;
      u32 y = closest->coords.y;

      auto neighbors = get_neighbors( *closest );

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
  NoiseMap GeneratePerlinNoise(
    float seed,
    int nOctaves,
    float
      scaling_bias// the lower the scaling bias, the more the higher octaves dominate
  ) {
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
          // @volatile
          // If we know our dimensions are always a power of 2
          // they can be halved quite easily
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
          fScale = fScale / scaling_bias;
        }

        // set the normalized perlin noise to the element in the output array
        fOutput[y * MAP_WIDTH + x] = ( fNoise / fScaleAcc );
      }

    NormalizeMap( fOutput );
    return fOutput;
  }

  void FilterIslands( NoiseMap &noiseMap, f32 water_lvl ) {
    for ( int x = 1; x < MAP_WIDTH - 1; x++ )
      for ( int y = 1; y < MAP_HEIGHT - 1; y++ ) {
        u32 numWater = 0;

        float NE = noiseMap.at( index( x, y - 1 ) );
        if ( NE < water_lvl )
          numWater++;

        float E = noiseMap.at( index( x + 1, y ) );
        if ( E < water_lvl )
          numWater++;

        float SE = noiseMap.at( index( x, y + 1 ) );
        if ( SE < water_lvl )
          numWater++;

        float SW = noiseMap.at( index( x - 1, y + 1 ) );
        if ( SW < water_lvl )
          numWater++;

        float W = noiseMap.at( index( x - 1, y ) );
        if ( W < water_lvl )
          numWater++;

        float NW = noiseMap.at( index( x - 1, y - 1 ) );
        if ( NW < water_lvl )
          numWater++;

        if ( numWater >= 5 ) {
          noiseMap[index( x, y )] = 0.0f;
        }
      }
  }

  // @unused
  void map_pwr_two( NoiseMap &noise ) {
    for ( u32 i = 0; i < MAP_WIDTH * MAP_WIDTH; i++ ) {
      noise[i] = noise[i] * noise[i];
    }
  }


  void NormalizeMap( NoiseMap &pNoise ) {
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
};
