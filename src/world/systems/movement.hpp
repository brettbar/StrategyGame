#pragma once

#include "../components/animated.hpp"
#include "../components/selected.hpp"
#include "../components/unit.hpp"

namespace Movement
{
inline void SetDestinations( entt::registry &registry, Camera2D camera )
{
  entt::basic_view view =
    registry.view<c_Unit::Unit, c_Animated::Animated, c_Selected::Selected>();

  for ( auto entity: view )
  {
    c_Unit::Unit &unit = view.get<c_Unit::Unit>( entity );
    c_Animated::Animated &anim = view.get<c_Animated::Animated>( entity );

    std::unique_ptr<Vector2> tileOrig =
      DetermineTilePos( GetScreenToWorld2D( GetMousePosition(), camera ) );

    if ( tileOrig != nullptr )
    {
      unit.destination = *tileOrig;

      if ( unit.destination.x > unit.position.x )
        anim.direction = c_Animated::IDLE_DR;
      else if ( unit.destination.x < unit.position.x )
        anim.direction = c_Animated::IDLE_DL;
    }
  }
}

inline void Update( entt::registry &registry, f32 timeScale )
{
  entt::basic_view units = registry.view<c_Unit::Unit, c_Animated::Animated>();

  for ( auto &entity: units )
  {
    c_Unit::Unit &unit = units.get<c_Unit::Unit>( entity );
    c_Animated::Animated &anim = units.get<c_Animated::Animated>( entity );

    if ( Vector2Distance( unit.destination, unit.position ) > 0.7f )
    {
      Vector2 unitVec = Vector2Normalize( {
        unit.destination.x - unit.position.x,
        unit.destination.y - unit.position.y,
      } );

      unit.position.x += unitVec.x * unit.speed * timeScale;
      unit.position.y += unitVec.y * unit.speed * timeScale;

      if ( unit.destination.x > unit.position.x )
        anim.state = c_Animated::WALK_DR;
      else if ( unit.destination.x < unit.position.x )
        anim.state = c_Animated::WALK_DL;

      if ( Vector2Distance( unit.destination, unit.position ) <= 0.7f )
      {
        unit.position = unit.destination;

        if ( anim.direction == 0 ) anim.state = c_Animated::IDLE_DR;
        else if ( anim.direction == 1 )
          anim.state = c_Animated::IDLE_DL;
      }
    }
  }
}

};// namespace Movement
