#pragma once

#include "../../shared/common.hpp"
#include <cereal/types/common.hpp>
#include <cereal/types/vector.hpp>

namespace Animated {

// TODO should this be an enum class?
enum AnimState {
  IDLE_DR,
  IDLE_DL,
  WALK_DR,
  WALK_DL,
};

struct Anim {
  AnimState name;
  u32 frameLength;
  f32 speed;

  template<class Archive>
  void serialize( Archive &ar ) {
    ar( name, frameLength, speed );
  }
};

using Animations = std::vector<Anim>;

struct Component {
  // Texture2D sprite;
  str sprite_id;
  Rectangle frameRec;
  AnimState state;
  Animations animations;
  u32 direction;
  u32 currFrame;
  f32 animTime;
  bool moving;

  template<class Archive>
  void serialize( Archive &ar ) {
    ar(
      CEREAL_NVP(sprite_id),
      // CEREAL_NVP(sprite.id),
      // CEREAL_NVP(sprite.width),
      // CEREAL_NVP(sprite.height),
      // CEREAL_NVP(sprite.mipmaps),
      // CEREAL_NVP(sprite.format),
      frameRec.x,
      frameRec.y,
      frameRec.width,
      frameRec.height,
      state,
      direction,
      currFrame,
      animTime,
      moving
    );
    ar( CEREAL_NVP( animations ) );
  }

};

}// namespace Animated
