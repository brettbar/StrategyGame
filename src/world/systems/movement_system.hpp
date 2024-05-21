// @todo fold into actor system

#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../../shared/utils.hpp"

#include "../components/actor_component.hpp"
#include "../components/animated_component.hpp"
#include "../components/selected_component.hpp"

namespace MovementSystem {

  // REFACTOR
  inline void SetDestinations( entt::entity entity, Vector2 click_pos ) {
    // entt::basic_view view =
    //   Global::world
    //     .view<Actor::Component, Animated::Component, Selected::Component>();

    // for ( auto entity: view )
    // {
    Actor::Component &actor = Global::world.get<Actor::Component>( entity );
    Animated::Component &anim =
      Global::world.get<Animated::Component>( entity );


    std::unique_ptr<Vector2> dest_tile = DetermineTilePos( click_pos );

    if ( dest_tile != nullptr ) {
      actor.destination = *dest_tile;

      if ( actor.destination.x > actor.position.x )
        anim.direction = Animated::IDLE_DR;
      else if ( actor.destination.x < actor.position.x )
        anim.direction = Animated::IDLE_DL;
    }
    // }
  }

  inline void Update(
    view<Actor::Component, Animated::Component> actors,
    f32 timeScale
  ) {
    for ( auto &entity: actors ) {
      Actor::Component &actor = actors.get<Actor::Component>( entity );
      Animated::Component &anim = actors.get<Animated::Component>( entity );

      if ( Vector2Distance( actor.destination, actor.position ) > 0.7f ) {
        Vector2 actorVec = Vector2Normalize( {
          actor.destination.x - actor.position.x,
          actor.destination.y - actor.position.y,
        } );

        actor.position.x += actorVec.x * actor.speed * timeScale;
        actor.position.y += actorVec.y * actor.speed * timeScale;

        // TODO maybe these 2 ifs could be consolidated
        if ( actor.destination.x > actor.position.x ) {
          anim.state = Animated::WALK_DR;
          actor.moving = true;
        } else if ( actor.destination.x < actor.position.x ) {
          anim.state = Animated::WALK_DL;
          actor.moving = true;
        }
        if ( actor.destination.x == actor.position.x ) {
          if ( anim.state == Animated::IDLE_DR ) {
            anim.state = Animated::WALK_DR;
            actor.moving = true;
          } else if ( anim.state == Animated::IDLE_DL ) {

            anim.state = Animated::WALK_DL;
            actor.moving = true;
          }
        }


        if ( Vector2Distance( actor.destination, actor.position ) <= 0.7f ) {
          actor.position = actor.destination;
          actor.moving = false;

          if ( anim.direction == 0 )
            anim.state = Animated::IDLE_DR;
          else if ( anim.direction == 1 )
            anim.state = Animated::IDLE_DL;
        }
      }
    }
  }

  inline bool ActorIsMoving( entt::entity entity ) {
    if ( Global::world.all_of<Actor::Component>( entity ) ) {
      return Global::world.get<Actor::Component>( entity ).moving;
    }
    return false;
  }

};// namespace MovementSystem
