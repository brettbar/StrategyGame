#pragma once

#ifndef MAP_HPP
#define MAP_HPP

#include "common.hpp"
#include <array>
#include <chrono>
#include <thread>

namespace Map {

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

  u32 neighborIds[6];
  // Vector2 neighborNE;
  // Vector2 neighborE;
  // Vector2 neighborSE;
  // Vector2 neighborSW;
  // Vector2 neighborW;
  // Vector2 neighborNW;
};


void CreateTerrain(entt::registry &, u32, u32);
void UpdateProvinces(entt::registry &);
void SetProvinceOwner(entt::registry &, u32, Vector2);

void DrawTerrain(entt::registry &, Texture2D, Rectangle);
void DrawProvinces(entt::registry &, bool, Texture2D);
void DrawSingleBorder(Tile);

Tile* FindTileByCoord(entt::registry &, u32, u32);

i32 determineTileIdFromClick(Vector2);
Vector2 *determineTilePos(Vector2);

}; // namespace Map

#endif
