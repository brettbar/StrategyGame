#pragma once

#include <array>
#include <chrono>
#include <thread>

#include "common.hpp"
#include "resource.hpp"

namespace Map
{

const u32 MAP_WIDTH = 128;
const u32 MAP_HEIGHT = 128;

struct Tile {
  u32 id;
  UVector2 coord;
  Vector2 position;
  Vector2 center;
  i32 owner;
  u32 population;
  str name;
};

struct TileMap {
  std::array<Tile, MAP_WIDTH * MAP_HEIGHT> tiles;
};

void CreateTerrain(entt::registry &, u32, u32);
u32 index(u32, u32);
void UpdateProvinces(entt::registry &);
void SetProvinceOwner(entt::registry &, u32, Vector2);

void DrawTerrain(entt::registry &, Texture2D, Rectangle);
void DrawProvinces(entt::registry &, TextureCache &);
void DrawSingleBorder(Tile);

Tile *FindTileByCoord(TileMap &, u32, u32);
};// namespace Map
