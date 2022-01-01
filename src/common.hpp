#pragma once

#include "../include/entt/entt.hpp"
#include <raylib.h>
#include <raymath.h>
#include <map>
#include <vector>

using u32 = unsigned int;
using i32 = int;
using f32 = float;
using str = std::string;
using hstr = entt::hashed_string;

struct IVector2 {
  i32 x;
  i32 y;
};

struct UVector2 {
  u32 x;
  u32 y;
};


struct State {
  u32 screenWidth;
  u32 screenHeight;
  const u32 mapWidth;
  const u32 mapHeight;
  f32 timeScale;
  f32 prevTimeScale;
  Camera2D camera;
  bool debug;
  
  Texture2D selectedTexture;
};

void PrintVec2(Vector2);
i32 DetermineTileIdFromClick(Vector2);
Vector2* DetermineTilePos(Vector2);
