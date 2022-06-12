#pragma once

#include "../common.hpp"

namespace Unit {

struct Component {
  Vector2 position;
  Vector2 destination;
  f32 speed;

  template<class Archive>
  void serialize( Archive &ar ) {
    ar( position.x, position.y, destination.x, destination.y, speed );
  }
};

}// namespace Unit
