//
// Created by brett on 1/30/2022.
//
#include "../components/actor.hpp"
#include "../global.hpp"
#include "map/terrain_system.hpp"
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
    !Global::registry.all_of<Unit::Component>(
      SelectionSystem::selected_entity ) )
    return false;


  // 1. if the colonist is moving, bail
  if ( MovementSystem::UnitIsMoving( SelectionSystem::selected_entity ) )
    return false;


  Unit::Component unit =
    Global::registry.get<Unit::Component>( SelectionSystem::selected_entity );

  i32 closest_tile = DetermineTileIdFromPosition( unit.position );

  // 2. if they aren't in a tile, bail
  if ( closest_tile == -1 )
    return false;

  for ( auto entity: Global::registry.view<Province::Component>() ) {
    Province::Component &prov =
      Global::registry.get<Province::Component>( entity );

    // !3. if the closest tile is owned by our faction, and the tile doesn't already have a settlement
    if (
      prov.tile->id == closest_tile && prov.owner == unit.owner &&
      !Global::registry.any_of<Settlement::Component>( entity ) ) {
      return true;
    }
  }

  // Otherwise, default to false
  return false;
}

};// namespace ActorSystem
