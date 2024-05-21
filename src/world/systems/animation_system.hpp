#pragma once

#include "../../shared/common.hpp"

#include "../components/actor_component.hpp"
#include "../components/animated_component.hpp"

#include "movement_system.hpp"

#include <raylib.h>

namespace AnimationSystem {

  inline void Draw(
    view<Actor::Component, Animated::Component> animated_actors,
    bool debug
  ) {

    // TODO maybe a better soln?
    Global::world.sort<Actor::Component>(
      []( const Actor::Component &lhs, const Actor::Component &rhs ) {
        return rhs.position.y > lhs.position.y;
      }
    );

    animated_actors.each( [debug](
                            Actor::Component &actor, Animated::Component &anim
                          ) {
      DrawTextureRec(
        anim.sprite,
        anim.frameRec,
        { actor.position.x - 64.0f, actor.position.y - 64.0f },
        WHITE
      );

      if ( debug && Vector2Distance( actor.position, actor.destination ) > 0.5f ) {
        DrawLineEx( actor.position, actor.destination, 2, MAGENTA );
      }
    } );
  }

  inline void Update(
    view<Actor::Component, Animated::Component> animated_actors,
    f32 timeScale
  ) {
    for ( auto &entity: animated_actors ) {
      Animated::Component &anim =
        animated_actors.get<Animated::Component>( entity );

      float animSpeed = 0.18f;
      anim.animTime +=
        anim.animations[anim.state].speed * timeScale * animSpeed;
      anim.currFrame =
        (int) anim.animTime % anim.animations[anim.state].frameLength;

      anim.frameRec.x = (f32) anim.currFrame * 128;
      anim.frameRec.y = anim.state * 128;
    }
  }

};// namespace AnimationSystem
