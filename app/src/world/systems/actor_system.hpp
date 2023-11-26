//
// Created by brett on 1/30/2022.
//
#pragma once

#include "../../shared/global.hpp"
#include "../components/actor.hpp"
#include "faction_system.hpp"
#include "map_system.hpp"
#include "movement_system.hpp"
#include "selection_system.hpp"
#include "settlement_system.hpp"

#include "../components/player.hpp"

namespace ActorSystem {

  // A settlement can be placed when
  // 0. A colonist is selected
  // 1. The colonist is not moving
  // 2. The colonist is in a province owned by their faction
  // 3. The province does not already contain a settlement
  inline bool ColonistCanPlaceSettlement() {
    entt::entity selected_entity = SelectionSystem::GetSelectedEntity();
    // 0. if the colonist isnt selected, bail
    if ( selected_entity == entt::null || !Global::world.all_of<Actor::Component>( selected_entity ) )
      return false;

    // 1. if the colonist is moving, bail
    if ( MovementSystem::ActorIsMoving( selected_entity ) )
      return false;

    Actor::Component actor =
      Global::world.get<Actor::Component>( selected_entity );

    i32 closest_tile = DetermineTileIdFromPosition( actor.position );

    // 2. if they aren't in a tile, bail
    if ( closest_tile == -1 )
      return false;

    for ( auto entity: Global::world.view<Province::Component>() ) {
      auto &prov = Global::world.get<Province::Component>( entity );

      // !3. if the closest tile is owned by our faction, and the tile doesn't already have a settlement
      if ( prov.tile->id == closest_tile && prov.tile->owner == actor.owner && !Global::world.any_of<Settlement::Component>( entity ) )
      {
        return true;
      }
    }

    // Otherwise, default to false
    return false;
  }

  // TODO(rf)
  // This probably has totally shit performance since we are basically spamming true false
  // interface updates.
  //
  // What would be better is to check the conditions every frame first and only toggle clickability
  // If there is a change
  inline void EvaluateActorActions() {
    if ( ColonistCanPlaceSettlement() ) {
      // std::cout << "ColonistCanPlace true" << '\n';
      InterfaceUpdate::Update{
        .id = InterfaceUpdate::ActorCanSpawnSettlement,
        .condition = true,
      }
        .Send();
    } else {
      // std::cout << "ColonistCanPlace false" << '\n';
      InterfaceUpdate::Update{
        .id = InterfaceUpdate::ActorCanSpawnSettlement,
        .condition = false,
      }
        .Send();
    }
  }

  inline void CreateColonist( entt::entity owner, Vector2 spawn ) {
    Texture2D sprite = FactionSystem::DetermineTextureFromFaction( owner );
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


  inline void SpawnColonist( entt::entity owner, Vector2 clickPos ) {
    std::unique_ptr<Vector2> spawn = DetermineTilePos( clickPos );
    assert( spawn != nullptr );

    ActorSystem::CreateColonist( owner, *spawn );
  }

  inline void SpawnColonist( entt::entity owner ) {
    std::unique_ptr<Vector2> spawn =
      std::make_unique<Vector2>( Vector2{ 64 * 64, 64 * 64 } );

    ActorSystem::CreateColonist( owner, *spawn );
  }

  inline void DeleteSelected() {
    auto selectedView =
      Global::world.view<Selected::Component, Actor::Component>();
    auto selectedEntity = selectedView.front();

    if ( selectedEntity == entt::null ) {
      printf( "No selected entity, cancelling\n" );
      return;
    }

    Global::world.destroy( selectedEntity );
  }

};// namespace ActorSystem
