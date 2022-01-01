#pragma once

#include "../include/entt/entt.hpp"
#include "player.hpp"
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

void PrintVec2(Vector2);
i32 DetermineTileIdFromClick(Vector2);
std::unique_ptr<Vector2> DetermineTilePos(Vector2);
