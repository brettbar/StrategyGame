#pragma once

#ifndef MAP_H
#define MAP_H

#include "common.h"
#include <chrono>
#include <thread>

using u32 = unsigned int;
using i32 = int;
using f32 = float;

namespace Map {

struct Tile {
  u32 id;
  Vector2 position;
  i32 owner;
  u32 population;
  std::string name;
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
void DrawBorder(Tile);

i32 determineTileIdFromClick(Vector2);
Vector2 *determineTilePos(Vector2);
}; // namespace Map

#endif
