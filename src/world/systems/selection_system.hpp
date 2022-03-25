#pragma once

#include "../../common.hpp"
#include "../components/event.hpp"
#include "../components/province.hpp"
#include "../components/selected.hpp"
#include "../components/unit.hpp"
#include "event_system.hpp"
#include <raylib.h>


namespace SelectionSystem {

inline void Draw( entt::registry &reg, bool isDebug ) {
  auto unitsView = reg.view<Selected::Component, Unit::Component>();
  auto provsView = reg.view<Selected::Component, Province::Component>();

  for ( auto entity: unitsView ) {
    Unit::Component &unit = unitsView.get<Unit::Component>( entity );
    if ( isDebug ) {
      DrawRectangleLinesEx(
        { unit.position.x - 32, unit.position.y - 32, 64, 64 },
        2,
        YELLOW );
    }
  }

  for ( auto entity: provsView ) {
    Province::Component &prov = provsView.get<Province::Component>( entity );
    if ( isDebug ) {
      DrawRectangleLinesEx(
        { prov.tile->position.x + 32, prov.tile->position.y + 32, 64, 64 },
        2,
        YELLOW );
    }
  }
}

inline void UpdateSelection( entt::registry &reg, Vector2 clickPos ) {
  auto unitsView = reg.view<Unit::Component>();
  auto provView = reg.view<Province::Component>();

  reg.clear<Selected::Component>();

  bool alreadyFoundOne = false;

  // use forward iterators and get only the components of interest
  for ( auto entity: unitsView ) {
    if ( alreadyFoundOne )
      return;

    Unit::Component &unit = unitsView.get<Unit::Component>( entity );

    if ( CheckCollisionPointCircle( unit.position, clickPos, 64 ) ) {
      reg.emplace<Selected::Component>( entity, true );
      alreadyFoundOne = true;

      printf( "selected an actor\n" );
      EventSystem::dispatcher.trigger<Event::SelectionEvent>(
        { .name = "Tulius Cicero" } );
    }
  }

  for ( auto entity: provView ) {
    if ( alreadyFoundOne )
      return;

    Province::Component &prov = provView.get<Province::Component>( entity );

    if ( CheckCollisionPointCircle(
           Vector2{ prov.tile->position.x + 32, prov.tile->position.y + 32 },
           clickPos,
           64 ) ) {
      if ( prov.settlement == nullptr )
        continue;


      reg.emplace<Selected::Component>( entity, true );
      alreadyFoundOne = true;

      printf( "selected a province\n" );
      EventSystem::dispatcher.trigger<Event::SelectionEvent>(
        { .name = prov.settlement->name } );
    }
  }
}

};// namespace SelectionSystem
