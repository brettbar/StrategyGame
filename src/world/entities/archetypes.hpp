//
// Created by brett on 12/31/2021.
//

#pragma once

#include "../../common.hpp"
#include "../../resource.hpp"
#include "../components/animated.hpp"
#include "../components/sight.hpp"
#include "../components/unit.hpp"

namespace Archetypes {

struct Actor {
  c_Unit::Unit unit;
  c_Animated::Animated animated;
  c_Sight::Sight sight;

  c_Animated::Animations animations;

  Actor( Texture2D sprite, Vector2 spawn ) {
    unit = c_Unit::Unit{
      .position = spawn,
      .destination = spawn,
      .speed = 1.0f,
    };
    animations = {
      { c_Animated::IDLE_DR, 2, 0.2f },
      { c_Animated::IDLE_DL, 2, 0.2f },
      { c_Animated::WALK_DL, 8, 0.8f },
      { c_Animated::WALK_DL, 8, 0.8f },
    };
    animated = c_Animated::Animated{
      .sprite = sprite,
      .frameRec = { 0, 0, 128, 128 },
      .state = c_Animated::IDLE_DR,
      .animations = animations,
      .direction = 0,
      .currFrame = 0,
      .animTime = 0.0f,
      .moving = false,
    };
    sight = c_Sight::Sight{
      .range = 1,
    };
  }
};


};// namespace Archetypes
