#pragma once

#ifndef MAP_H
#define MAP_H

#include "../include/entt.hpp"
#include "raylib.h"
#include "raymath.h"

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
void DrawTerrain(entt::registry &, Texture2D, Rectangle);
void UpdateProvinces(entt::registry &, Vector2);
void DrawProvinces(entt::registry &, Texture2D);
void DrawBorder(Tile);
Vector2 *determineTilePos(Vector2);
}; // namespace Map

#endif
