#pragma once

#include "../common.hpp"
#include "../components/animated.hpp"
#include "../components/unit.hpp"
#include "movement_system.hpp"
#include <raylib.h>

namespace AnimationSystem {

inline void Draw( bool debug ) {
  entt::basic_view villagers =
    Global::world.view<Unit::Component, Animated::Component>();

  Global::world.sort<Unit::Component>(
    []( const Unit::Component &lhs, const Unit::Component &rhs ) {
      return rhs.position.y > lhs.position.y;
    } );

  villagers.each( [debug]( Unit::Component &unit, Animated::Component &anim ) {
    DrawTextureRec(
      anim.sprite,
      anim.frameRec,
      { unit.position.x - 64.0f, unit.position.y - 64.0f },
      WHITE );

    if ( debug && Vector2Distance( unit.position, unit.destination ) > 0.5f ) {
      DrawLineEx( unit.position, unit.destination, 2, MAGENTA );
    }
  } );
}

inline void Update( f32 timeScale ) {
  auto view = Global::world.view<Animated::Component, Unit::Component>();

  for ( auto &entity: view ) {
    Animated::Component &anim = view.get<Animated::Component>( entity );

    float animSpeed = 0.18f;
    anim.animTime += anim.animations[anim.state].speed * timeScale * animSpeed;
    anim.currFrame =
      (int) anim.animTime % anim.animations[anim.state].frameLength;

    anim.frameRec.x = (f32) anim.currFrame * 128;
    anim.frameRec.y = anim.state * 128;
  }
}

};// namespace AnimationSystem
