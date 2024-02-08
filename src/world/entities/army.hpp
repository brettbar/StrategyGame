#pragma once

#include "../../shared/global.hpp"
#include "../components/actor.hpp"
#include "../components/animated.hpp"
#include "../components/sight.hpp"
#include "../systems/faction_system.hpp"

namespace Entities {

  inline static void create_army( entt::entity owner, vec2f spawn ) {
    Texture2D sprite = FactionSystem::hastati_texure_from_owner( owner );
    entt::entity entity = Global::world.create();
    Actor::Component actor = {
      .name = "Marcus Priscus",
      .type = Actor::Type::Colonist,
      .owner = owner,
      .position = spawn,
      .destination = spawn,
      .speed = 1.0f,
    };

    Animated::Animations animations = {
      { Animated::AnimState::IDLE_DR, 2, 0.2f },
      { Animated::AnimState::IDLE_DL, 2, 0.2f },
      { Animated::AnimState::WALK_DL, 8, 0.8f },
      { Animated::AnimState::WALK_DL, 8, 0.8f },
    };

    Animated::Component animated = {
      .sprite = sprite,
      .frameRec = { 0, 0, 128, 128 },
      .state = Animated::AnimState::IDLE_DR,
      .animations = animations,
      .direction = 0,
      .currFrame = 0,
      .animTime = 0.0f,
      .moving = false,
    };

    Sight::Component sight = {
      .range = 1,
    };

    Global::world.emplace<Actor::Component>( entity, actor );
    Global::world.emplace<Animated::Component>( entity, animated );
    Global::world.emplace<Sight::Component>( entity, sight );
  }

};// namespace Entities
