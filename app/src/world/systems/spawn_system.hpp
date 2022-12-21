// TODO this shouldnt be its own system. Move the spawns to their respective systems.

#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../components/actor.hpp"
#include "../components/animated.hpp"
#include "../components/event.hpp"
#include "../components/selected.hpp"
#include "../components/sight.hpp"
#include "../components/unit.hpp"

#include "actor_system.hpp"
#include "selection_system.hpp"

namespace SpawnSystem {

inline void SpawnColonist( entt::entity, Vector2 );
inline void SpawnColonist( entt::entity );
inline void DeleteSelected();


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
    Global::world.view<Selected::Component, Unit::Component>();
  auto selectedEntity = selectedView.front();

  if ( selectedEntity == entt::null ) {
    printf( "No selected entity, cancelling\n" );
    return;
  }

  Global::world.destroy( selectedEntity );
}

};// namespace SpawnSystem
