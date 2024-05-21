#pragma once

#include "../../shared/global.hpp"

#include "../components/animated_component.hpp"
#include "../components/faction_component.hpp"
#include "../components/sight_component.hpp"

#include "../managers/actor_manager.hpp"

namespace Actor {
  inline void create_army( entt::entity owner, vec2f spawn ) {
    str faction_id = Global::world.get<Faction::Component>( owner ).id;

    entt::entity entity = Global::world.create();
    Actor::Component actor = {
      .owner = owner,
      .data = Actor::Manager::Get()->roster.at( faction_id + "_army_tier_i" ),
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
      .sprite =
        Global::texture_cache[hstr{ actor.data.sprite_id.c_str() }]->texture,
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

  inline void create_colonist(
    entt::entity owner,
    Vector2 spawn = Vector2{ 64 * 64, 64 * 64 }
  ) {
    str faction_id = Global::world.get<Faction::Component>( owner ).id;

    entt::entity entity = Global::world.create();
    Actor::Component actor = {
      .owner = owner,
      .data = Actor::Manager::Get()->roster.at( faction_id + "_colonist" ),
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
      .sprite =
        Global::texture_cache[hstr{ actor.data.sprite_id.c_str() }]->texture,
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
}// namespace Actor