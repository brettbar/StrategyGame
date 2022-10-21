//
// Created by brett on 1/30/2022.
//
#include "../components/actor.hpp"
#include "../global.hpp"
#include "map/terrain_system.hpp"
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

  if ( !Global::registry.all_of<Unit::Component>(
         SelectionSystem::selected_entity ) )
    return false;

  if ( MovementSystem::UnitIsMoving( SelectionSystem::selected_entity ) )
    return false;


  Unit::Component unit =
    Global::registry.get<Unit::Component>( SelectionSystem::selected_entity );


  i32 closest_tile = DetermineTileIdFromPosition( unit.position );

  if ( closest_tile == -1 )
    return false;

  for ( auto entity: Global::registry.view<Province::Component>() ) {
    Province::Component &prov =
      Global::registry.get<Province::Component>( entity );

    if ( prov.tile->id == closest_tile ) {
      if ( prov.owner == unit.owner ) {
        return true;
      }
    }
  }

  return false;
}

};// namespace ActorSystem
