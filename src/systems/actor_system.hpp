//
// Created by brett on 1/30/2022.
//
#include "../components/actor.hpp"
#include "../global.hpp"
#include "movement_system.hpp"
#include "selection_system.hpp"

namespace ActorSystem {

// A settlement can be placed when
// 0. A colonist is selected
// 1. The colonist is not moving
// 2. The colonist is in a province owned by their faction
// 3. The province does not already contain a settlement
inline bool ColonistCanPlaceSettlement() {
  if ( SelectionSystem::selected_entity == entt::null )
    return false;

  return !MovementSystem::UnitIsMoving( SelectionSystem::selected_entity );
}

};// namespace ActorSystem
