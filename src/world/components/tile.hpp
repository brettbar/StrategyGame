#pragma once

#include "../../common.hpp"

namespace Tile {

enum Visibility {
  UNEXPLORED,
  EXPLORED,
  VISIBILE,
};

enum Biome {
  WATER,
  BEACH,
  LAND,
  HILLS,
  MTNS,
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
