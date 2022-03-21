#pragma once

#include "../../common.hpp"
#include "../components/animated.hpp"
#include "../components/unit.hpp"
#include "movement.hpp"

namespace Animation {

// void Draw(entt::registry &, bool);
// void UpdateSprites(entt::registry &, f32);

inline void Draw( entt::registry &registry, bool debug ) {
  entt::basic_view villagers =
    registry.view<c_Unit::Unit, c_Animated::Animated>();

  registry.sort<c_Unit::Unit>(
    []( const c_Unit::Unit &lhs, const c_Unit::Unit &rhs ) {
      return rhs.position.y > lhs.position.y;
    } );

  villagers.each( [debug]( c_Unit::Unit &unit, c_Animated::Animated &anim ) {
    //    DrawTextureV(
    //        unit.sprite,
    //        {unit.position.x - 64.0f, unit.position.y - 64.0f},
    //        WHITE);

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

inline void UpdateSprites( entt::registry &reg, f32 timeScale ) {
  auto view = reg.view<c_Animated::Animated, c_Unit::Unit>();

  for ( auto &entity: view ) {
    c_Animated::Animated &anim = view.get<c_Animated::Animated>( entity );

    float animSpeed = 0.18f;
    anim.animTime += anim.animations[anim.state].speed * timeScale * animSpeed;
    anim.currFrame =
      (int) anim.animTime % anim.animations[anim.state].frameLength;

    anim.frameRec.x = (f32) anim.currFrame * 128;
    anim.frameRec.y = anim.state * 128;
  }
}

};// namespace Animation
