#pragma once

#include "../common.hpp"
#include "../data/biomes.hpp"

namespace Tile {

enum class Visibility {
  UNEXPLORED,
  EXPLORED,
  VISIBILE,
};

struct Component {
  u32 id;
  f32 noise;
  Vector2 position;
  Vector2 center;
  UVector2 coords;
  Biome biome;
  Visibility visibility;
};


};// namespace Tile
