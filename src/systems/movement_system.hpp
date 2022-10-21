#pragma once

#include "../components/animated.hpp"
#include "../components/selected.hpp"
#include "../components/unit.hpp"
#include "../global.hpp"

namespace MovementSystem {

inline void SetDestinations( Camera2D camera ) {
  entt::basic_view view =
    Global::registry
      .view<Unit::Component, Animated::Component, Selected::Component>();

  for ( auto entity: view ) {
    Unit::Component &unit = view.get<Unit::Component>( entity );
    Animated::Component &anim = view.get<Animated::Component>( entity );

    std::unique_ptr<Vector2> tileOrig =
      DetermineTilePos( GetScreenToWorld2D( GetMousePosition(), camera ) );

    if ( tileOrig != nullptr ) {
      unit.destination = *tileOrig;

      if ( unit.destination.x > unit.position.x )
        anim.direction = Animated::IDLE_DR;
      else if ( unit.destination.x < unit.position.x )
        anim.direction = Animated::IDLE_DL;
    }
  }
}

inline void Update( f32 timeScale ) {
  entt::basic_view units =
    Global::registry.view<Unit::Component, Animated::Component>();

  for ( auto &entity: units ) {
    Unit::Component &unit = units.get<Unit::Component>( entity );
    Animated::Component &anim = units.get<Animated::Component>( entity );

    if ( Vector2Distance( unit.destination, unit.position ) > 0.7f ) {
      Vector2 unitVec = Vector2Normalize( {
        unit.destination.x - unit.position.x,
        unit.destination.y - unit.position.y,
      } );

      unit.position.x += unitVec.x * unit.speed * timeScale;
      unit.position.y += unitVec.y * unit.speed * timeScale;

      if ( unit.destination.x > unit.position.x )
        anim.state = Animated::WALK_DR;
      else if ( unit.destination.x < unit.position.x )
        anim.state = Animated::WALK_DL;

      if ( unit.destination.x == unit.position.x ) {
        if ( anim.state == Animated::IDLE_DR )
          anim.state = Animated::WALK_DR;
        else if ( anim.state == Animated::IDLE_DL )
          anim.state = Animated::WALK_DL;
      }


      if ( Vector2Distance( unit.destination, unit.position ) <= 0.7f ) {
        unit.position = unit.destination;

        if ( anim.direction == 0 )
          anim.state = Animated::IDLE_DR;
        else if ( anim.direction == 1 )
          anim.state = Animated::IDLE_DL;
      }
    }
  }
}

};// namespace MovementSystem
