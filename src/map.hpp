#pragma once

#ifndef MAP_HPP
#define MAP_HPP

#include "common.hpp"
#include <chrono>
#include <thread>

namespace Map {

struct Tile {
  u32 id;
  Vector2 coord;
  Vector2 position;
  i32 owner;
  u32 population;
  str name;

  std::vector<Vector2> neighborCoords;
  // Vector2 neighborNE;
  // Vector2 neighborE;
  // Vector2 neighborSE;
  // Vector2 neighborSW;
  // Vector2 neighborW;
  // Vector2 neighborNW;
};

struct IVector2 {
  i32 x;
  i32 y;
};

void CreateTerrain(entt::registry &, u32, u32);
void UpdateProvinces(entt::registry &);
void SetProvinceOwner(entt::registry &, u32, Vector2);

void DrawTerrain(entt::registry &, Texture2D, Rectangle);
void DrawProvinces(entt::registry &, bool, Texture2D);
void DrawBorders(entt::registry &);
void DrawSingleBorder(Tile);

i32 determineTileIdFromClick(Vector2);
Vector2 *determineTilePos(Vector2);

}; // namespace Map

#endif
