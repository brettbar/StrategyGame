#pragma once

#ifndef MAP_H
#define MAP_H

#include "../include/entt.hpp"
#include "raylib.h"

using u32 = unsigned int;
using i32 = int;
using f32 = float;

namespace Map {
void Create(entt::registry &, u32, u32);
void Draw(entt::registry &, Texture2D, Rectangle);
}; // namespace Map

#endif
