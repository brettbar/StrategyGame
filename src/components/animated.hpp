#pragma once
#include "../common.hpp"

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
};

using Animations = std::vector<Anim>;

struct Animated {
  Texture2D sprite;
  Rectangle frameRec;
  AnimState state;
  Animations animations;
  u32 currFrame;
  f32 animTime;
};
