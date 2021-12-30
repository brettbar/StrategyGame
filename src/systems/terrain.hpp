//
// Created by brett on 12/29/2021.
//
#pragma once

#include "../common.hpp"

#include <chrono>

namespace Terrain
{
  const u32 MAP_WIDTH = 128;
  const u32 MAP_HEIGHT = 128;

  enum Biome {
    WATER, BEACH, LAND, HILLS, MTNS,
  };

  struct Tile {
    u32 id;
    f32 noise;
    Vector2 position;
    Vector2 center;
    Biome biome;
  };

  using NoiseMap = std::array<float, MAP_WIDTH * MAP_HEIGHT>;
  using TileMap = std::array<Tile *, Terrain::MAP_WIDTH * Terrain::MAP_HEIGHT>;

  void CreateTerrain(entt::registry &, u32, u32);
  void CreateFOW(entt::registry &);

  void DrawTerrain(entt::registry &, Texture2D, Rectangle);

  NoiseMap GeneratePerlinNoise(float *, int, float);
  void FilterIslands(NoiseMap &);

  u32 index(u32, u32);

};// namespace Terrain
