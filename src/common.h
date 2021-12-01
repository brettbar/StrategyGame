
#pragma once

#ifndef COMMON_H 
#define COMMON_H

#include "../include/entt/entt.hpp"
#include <raylib.h>
#include <raymath.h>
#include <map>
#include <vector>

using u32 = unsigned int;
using i32 = int;
using f32 = float;
using str = std::string;

struct State {
  entt::registry registry;
  u32 screenWidth;
  u32 screenHeight;
  const u32 mapWidth;
  const u32 mapHeight;
  f32 timeScale;
  f32 prevTimeScale;
  std::map<str, Texture2D> textures;
  Camera2D camera;
  bool debug;
  
  Texture2D selectedTexture;
};

void PrintVec2(Vector2);

#endif
