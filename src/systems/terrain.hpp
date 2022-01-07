//
// Created by brett on 12/29/2021.
//
#pragma once

#include "../common.hpp"
#include "../resource.hpp"
#include "../components/sight.hpp"
#include "../components/unit.hpp"
#include "rlgl.h"
#include <chrono>

namespace Terrain
{
  const u32 MAP_WIDTH = 128;
  const u32 MAP_HEIGHT = 128;

  enum Visibility
  {
    UNEXPLORED,
    EXPLORED,
    VISIBILE,
  };

  enum Biome
  {
    WATER,
    BEACH,
    LAND,
    HILLS,
    MTNS,
  };

  struct Tile {
    u32 id;
    f32 noise;
    Vector2 position;
    Vector2 center;
    UVector2 coords;
    Biome biome;
    Visibility visibility;
  };

  using NoiseMap = std::array<float, MAP_WIDTH * MAP_HEIGHT>;
  using TileMap = std::array<std::shared_ptr<Tile>,
                             Terrain::MAP_WIDTH * Terrain::MAP_HEIGHT>;

  void CreateTerrain(entt::registry &);

  void DrawTerrain(entt::registry &, TextureCache &);
  void UpdateFOW(entt::registry &);

  NoiseMap GeneratePerlinNoise(float, int, float);
  void FilterIslands(NoiseMap &, float);

  u32 index(u32, u32);

};// namespace Terrain
