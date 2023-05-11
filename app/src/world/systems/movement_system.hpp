#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../../shared/utils.hpp"
#include "../components/animated.hpp"
#include "../components/selected.hpp"
#include "../components/unit.hpp"

namespace MovementSystem
{

  // REFACTOR
  inline void SetDestinations( Vector2 click_pos )
  {
    entt::basic_view view =
      Global::world
        .view<Unit::Component, Animated::Component, Selected::Component>();

    for ( auto entity: view )
    {
      Unit::Component &unit = view.get<Unit::Component>( entity );
      Animated::Component &anim = view.get<Animated::Component>( entity );


      std::unique_ptr<Vector2> dest_tile = DetermineTilePos( click_pos );

      if ( dest_tile != nullptr )
      {
        unit.destination = *dest_tile;

        if ( unit.destination.x > unit.position.x )
          anim.direction = Animated::IDLE_DR;
        else if ( unit.destination.x < unit.position.x )
          anim.direction = Animated::IDLE_DL;
      }
    }
  }

  inline void Update(
    View<Unit::Component, Animated::Component> units,
    f32 timeScale
  )
  {
    for ( auto &entity: units )
    {
      Unit::Component &unit = units.get<Unit::Component>( entity );
      Animated::Component &anim = units.get<Animated::Component>( entity );

      if ( Vector2Distance( unit.destination, unit.position ) > 0.7f )
      {
        Vector2 unitVec = Vector2Normalize( {
          unit.destination.x - unit.position.x,
          unit.destination.y - unit.position.y,
        } );

        unit.position.x += unitVec.x * unit.speed * timeScale;
        unit.position.y += unitVec.y * unit.speed * timeScale;

        // TODO maybe these 2 ifs could be consolidated
        if ( unit.destination.x > unit.position.x )
        {
          anim.state = Animated::WALK_DR;
          unit.moving = true;
        }
        else if ( unit.destination.x < unit.position.x )
        {
          anim.state = Animated::WALK_DL;
          unit.moving = true;
        }
        if ( unit.destination.x == unit.position.x )
        {
          if ( anim.state == Animated::IDLE_DR )
          {
            anim.state = Animated::WALK_DR;
            unit.moving = true;
          }
          else if ( anim.state == Animated::IDLE_DL )
          {

            anim.state = Animated::WALK_DL;
            unit.moving = true;
          }
        }


        if ( Vector2Distance( unit.destination, unit.position ) <= 0.7f )
        {
          unit.position = unit.destination;
          unit.moving = false;

          if ( anim.direction == 0 )
            anim.state = Animated::IDLE_DR;
          else if ( anim.direction == 1 )
            anim.state = Animated::IDLE_DL;
        }
      }
    }
  }

  inline bool UnitIsMoving( entt::entity entity )
  {
    if ( Global::world.all_of<Unit::Component>( entity ) )
    {
      return Global::world.get<Unit::Component>( entity ).moving;
    }
    return false;
  }

};// namespace MovementSystem
