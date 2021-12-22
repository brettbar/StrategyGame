#pragma once
#include "../common.hpp"


struct Unit {
  Vector2 position;
  Vector2 destination;
  f32 speed;
};

struct Selected {
  bool isSelected;
};
