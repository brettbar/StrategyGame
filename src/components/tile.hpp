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

  template<class Archive>
  void serialize( Archive &ar ) {
    ar(
      id,
      noise,
      position.x,
      position.y,
      center.x,
      center.y,
      coords.x,
      coords.y,
      biome,
      visibility
    );
  }
};


};// namespace Tile
