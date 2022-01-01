//
// Created by brett on 12/31/2021.
//

#pragma once

#include "../common.hpp"
#include "../resource.hpp"
#include "../components/animated.hpp"
#include "../components/unit.hpp"
#include "../components/sight.hpp"

namespace Archetypes
{

  struct Actor
  {

    Unit unit;
    Animated animated;
    Sight sight;

    Animations animations;

    Actor(Texture2D sprite, Vector2 spawn)
    {
      unit = Unit{
        .position = spawn,
        .destination = spawn,
        .speed = 1.0f,
      };
      animations = {
        {IDLE_DR, 2, 0.2f},
        {IDLE_DL, 2, 0.2f},
        {WALK_DL, 8, 0.8f},
        {WALK_DL, 8, 0.8f},
      };
      animated = Animated{
        .sprite = sprite,
        .frameRec = {0, 0, 128, 128},
        .state = IDLE_DR,
        .animations = animations,
        .direction = 0,
        .currFrame = 0,
        .animTime = 0.0f,
        .moving = false,
      };
      sight = Sight{
        .range = 1,
      };
    }

  };


};// namespace Archetypes