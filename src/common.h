
#pragma once

#ifndef COMMON_H 
#define COMMON_H

#include "../include/entt/entt.hpp"
#include <raylib.h>
#include <raymath.h>
#include <map>

using u32 = unsigned int;
using i32 = int;
using f32 = float;

struct State {
  entt::registry registry;
  u32 screenWidth;
  u32 screenHeight;
  const u32 mapWidth;
  const u32 mapHeight;
  f32 timeScale;
  f32 prevTimeScale;
  std::map<std::string, Texture2D> textures;
  Camera2D camera;
  bool debug;
};

#endif
