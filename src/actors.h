#pragma once

#ifndef ACTORS_H
#define ACTORS_H

#include "../include/entt.hpp"
#include "raylib.h"
#include "raymath.h"

using u32 = unsigned int;
using i32 = int;
using f32 = float;

namespace Actors {

struct RomanVillager {
  Vector2 position;
  Vector2 destination;
  f32 speed;
  Texture2D sprite;
};

struct Selected {
  bool isSelected;
};

void Draw(entt::registry&);
void CreateNew(entt::registry&, Vector2, Texture2D);
void UpdateSelection(entt::registry&, Vector2);
void SetDestinations(entt::registry&, Camera2D);
void UpdateMovement(entt::registry&);
Vector2* determineTilePos(Vector2);
}; // namespace Units

#endif
