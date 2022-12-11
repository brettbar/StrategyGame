#pragma once

#include "../common.hpp"
#include "../components/animated.hpp"
#include "../components/unit.hpp"
#include "movement_system.hpp"
#include <raylib.h>

namespace AnimationSystem {

inline void
Draw( View<Unit::Component, Animated::Component> animated_units, bool debug ) {

  // TODO maybe a better soln?
  Global::world.sort<Unit::Component>(
    []( const Unit::Component &lhs, const Unit::Component &rhs ) {
      return rhs.position.y > lhs.position.y;
    }
  );

  animated_units.each( [debug](
                         Unit::Component &unit, Animated::Component &anim
                       ) {
    DrawTextureRec(
      anim.sprite,
      anim.frameRec,
      { unit.position.x - 64.0f, unit.position.y - 64.0f },
      WHITE
    );

    if ( debug && Vector2Distance( unit.position, unit.destination ) > 0.5f ) {
      DrawLineEx( unit.position, unit.destination, 2, MAGENTA );
    }
  } );
}

inline void Update(
  View<Unit::Component, Animated::Component> animated_units,
  f32 timeScale
) {
  for ( auto &entity: animated_units ) {
    Animated::Component &anim =
      animated_units.get<Animated::Component>( entity );

    float animSpeed = 0.18f;
    anim.animTime += anim.animations[anim.state].speed * timeScale * animSpeed;
    anim.currFrame =
      (int) anim.animTime % anim.animations[anim.state].frameLength;

    anim.frameRec.x = (f32) anim.currFrame * 128;
    anim.frameRec.y = anim.state * 128;
  }
}

};// namespace AnimationSystem
