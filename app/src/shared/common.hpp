#pragma once

// TODO see which of these includes can be removed
#include <entt/entt.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>


using ECS = entt::registry;
using u32 = unsigned int;
using i32 = int;
using f32 = float;
// using str = std::string;
using hstr = entt::hashed_string;
using vec2 = Vector2;
using rect = Rectangle;

template<typename T>
using ptr = std::shared_ptr<T>;

template<typename... T>
using View =
  entt::basic_view<entt::get_t<entt::storage_for_t<T>...>, entt::exclude_t<>>;

struct IVector2 {
  i32 x;
  i32 y;
};

struct UVector2 {
  u32 x;
  u32 y;
};

const f32 TILE_WIDTH = 64;
const f32 TILE_HEIGHT = 64;
const u32 MAP_WIDTH = 128;
const u32 MAP_HEIGHT = 128;
