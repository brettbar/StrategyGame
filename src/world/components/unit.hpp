#pragma once

#include "components.hpp"

namespace Components
{
  struct Unit {
    Vector2 position;
    Vector2 destination;
    f32 speed;
  };

  struct Selected {
    bool isSelected;
  };
}// namespace Components
