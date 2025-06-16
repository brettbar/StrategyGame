#pragma once

// TODO see which of these includes can be removed
#include <entt/entt.hpp>
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>


#define LEN(arr) sizeof(arr) / sizeof(arr[0])

using ECS = entt::registry;
using u32 = unsigned int;
using i32 = int;
using f32 = float;
using u64 = unsigned long long;
using str = std::string;
using cstr = const char *;
using hstr = entt::hashed_string;
using vec2f = Vector2;
using rect = Rectangle;

template<typename T>
using opt = std::optional<T>;

struct vec2i {
  i32 x;
  i32 y;
};

struct vec2u {
  u32 x;
  u32 y;
};
struct urect {
  u32 x;
  u32 y;
  u32 z;
  u32 w;

  u32 width() {
    return (z - x) + 1;
  }

  u32 height() {
    return (w - y) + 1;
  }

  vec2u center() {
    return {
      x + width() / 2,
      y + height() / 2,
    };
  }
};

template<typename K, typename V>
using map = std::map<K, V>;

template<typename T>
using func = std::function<T>;

template<typename T>
using list = std::vector<T>;

template<typename T>
using sptr = std::shared_ptr<T>;

template<typename T>
using uptr = std::unique_ptr<T>;

template<typename... T>
using view =
  entt::basic_view<entt::get_t<entt::storage_for_t<T>...>, entt::exclude_t<>>;


const f32 TILE_WIDTH = 64;
const f32 TILE_HEIGHT = 64;
const u32 MAP_WIDTH = 128;
const u32 MAP_HEIGHT = 128;


inline std::map<std::string, Color> color_map = {
  {"red", RED},
  {"gold", GOLD},
  {"cyan", BLUE},
  {"green", GREEN},
  {"white", WHITE},
  {"purple", PURPLE},
  {"orange", ORANGE},
  {"pink", PINK},
  {"grey", GRAY},
};
