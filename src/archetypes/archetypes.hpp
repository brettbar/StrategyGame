//
// Created by brett on 12/31/2021.
//

#pragma once

#include "../common.hpp"
#include "../components/animated.hpp"
#include "../components/sight.hpp"
#include "../components/unit.hpp"
#include "../renderer/textures.hpp"

namespace Archetypes {

struct Character {
  Unit::Component unit;
  Animated::Component animated;
  Sight::Component sight;

  Animated::Animations animations;

  Character( Texture2D sprite, Vector2 spawn ) {
    unit = Unit::Component{
      .position = spawn,
      .destination = spawn,
      .speed = 1.0f,
    };
    animations = {
      { Animated::AnimState::IDLE_DR, 2, 0.2f },
      { Animated::AnimState::IDLE_DL, 2, 0.2f },
      { Animated::AnimState::WALK_DL, 8, 0.8f },
      { Animated::AnimState::WALK_DL, 8, 0.8f },
    };
    animated = Animated::Component{
      .sprite = sprite,
      .frameRec = { 0, 0, 128, 128 },
      .state = Animated::AnimState::IDLE_DR,
      .animations = animations,
      .direction = 0,
      .currFrame = 0,
      .animTime = 0.0f,
      .moving = false,
    };
    sight = Sight::Component{
      .range = 1,
    };
  }
};


};// namespace Archetypes
