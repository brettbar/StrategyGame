#pragma once

#ifndef ACTORS_HPP
#define ACTORS_HPP

#include "common.hpp"
#include "map.hpp"

namespace Actors {

struct Unit {
  Vector2 position;
  Vector2 destination;
  f32 speed;
  Texture2D sprite;
};

struct Selected {
  bool isSelected;
};

void Draw(entt::registry&, bool);
void CreateNew(entt::registry&, Vector2, u32, std::map<str, Texture2D>);
void CreateNew(entt::registry&, Vector2, Texture2D);
void UpdateSelection(entt::registry&, Vector2);
void SetDestinations(entt::registry&, Camera2D);
void UpdateMovement(entt::registry&, f32);
Vector2* determineTilePos(Vector2);
}; // namespace Units

#endif
