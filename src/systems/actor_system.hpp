//
// Created by brett on 1/30/2022.
//
#pragma once

#include "../components/actor.hpp"
#include "../components/player.hpp"
#include "../global.hpp"
#include "map_system.hpp"
#include "movement_system.hpp"
#include "selection_system.hpp"
#include "settlement_system.hpp"

namespace ActorSystem {

// A settlement can be placed when
// 0. A colonist is selected
// 1. The colonist is not moving
// 2. The colonist is in a province owned by their faction
// 3. The province does not already contain a settlement
inline bool ColonistCanPlaceSettlement() {
  // 0. if the colonist isnt selected, bail
  if (
    SelectionSystem::selected_entity == entt::null ||
    !Global::world.all_of<Unit::Component>( SelectionSystem::selected_entity ) )
    return false;


  // 1. if the colonist is moving, bail
  if ( MovementSystem::UnitIsMoving( SelectionSystem::selected_entity ) )
    return false;


  Unit::Component unit =
    Global::world.get<Unit::Component>( SelectionSystem::selected_entity );

  i32 closest_tile = DetermineTileIdFromPosition( unit.position );

  // 2. if they aren't in a tile, bail
  if ( closest_tile == -1 )
    return false;

  for ( auto entity: Global::world.view<Province::Component>() ) {
    Province::Component &prov =
      Global::world.get<Province::Component>( entity );

    // !3. if the closest tile is owned by our faction, and the tile doesn't already have a settlement
    if (
      prov.tile->id == closest_tile && prov.owner == unit.owner &&
      !Global::world.any_of<Settlement::Component>( entity ) ) {
      return true;
    }
  }

  // Otherwise, default to false
  return false;
}

inline Texture2D DetermineTextureFromFaction( entt::entity owner ) {
  switch ( Global::world.get<Player::Component>( owner ).faction ) {
    case Faction::ID::Romans:
      return Global::texture_cache[hstr{ "romanVillagerTexture" }]->texture;
    case Faction::ID::Greeks:
      return Global::texture_cache[hstr{ "greekVillagerTexture" }]->texture;
    case Faction::ID::Celts:
      return Global::texture_cache[hstr{ "celtVillagerTexture" }]->texture;
  }

  return Global::texture_cache[hstr{ "romanVillagerTexture" }]->texture;
}

inline void CreateColonist( entt::entity owner, Vector2 spawn ) {
  Texture2D sprite = DetermineTextureFromFaction( owner );
  entt::entity entity = Global::world.create();
  Unit::Component unit = {
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

  Global::world.emplace<Actor::Component>(
    entity,
    "Marcus Priscus",
    Actor::Type::Colonist );
  Global::world.emplace<Unit::Component>( entity, unit );
  Global::world.emplace<Animated::Component>( entity, animated );
  Global::world.emplace<Sight::Component>( entity, sight );
}

};// namespace ActorSystem
