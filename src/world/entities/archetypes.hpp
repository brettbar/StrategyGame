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
    Components::Unit unit;
    Components::Animated animated;
    Components::Sight sight;

    Components::Animations animations;

    Actor(Texture2D sprite, Vector2 spawn) {
      unit = Components::Unit{
        .position = spawn,
        .destination = spawn,
        .speed = 1.0f,
      };
      animations = {
        {Components::IDLE_DR, 2, 0.2f},
        {Components::IDLE_DL, 2, 0.2f},
        {Components::WALK_DL, 8, 0.8f},
        {Components::WALK_DL, 8, 0.8f},
      };
      animated = Components::Animated{
        .sprite = sprite,
        .frameRec = {0, 0, 128, 128},
        .state = Components::IDLE_DR,
        .animations = animations,
        .direction = 0,
        .currFrame = 0,
        .animTime = 0.0f,
        .moving = false,
      };
      sight = Components::Sight{
        .range = 1,
      };
    }
  };


};// namespace Archetypes
