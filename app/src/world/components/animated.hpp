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
  Texture2D sprite;
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
      sprite.id,
      sprite.width,
      sprite.height,
      sprite.mipmaps,
      sprite.format,
      frameRec.x,
      frameRec.y,
      frameRec.width,
      frameRec.height,
      state,
      animations,
      direction,
      currFrame,
      animTime,
      moving
    );
  }

  template<class Archive>
  void serialize( Archive &ar, Animations &anims ) {
    ar( CEREAL_NVP( anims ) );
  }
};

}// namespace Animated
