#pragma once
#include "../common.hpp"

enum AnimState
{
  IDLE_DR,
  IDLE_DL,
  WALK_DR,
  WALK_DL,
};

struct Animation {
  AnimState name;
  u32 frameLength;
  f32 speed;
};

using Animations = std::vector<Animation>;

struct Animated {
  Texture2D sprite;
  Rectangle frameRec;
  AnimState state;
  Animations animations;
  u32 direction;
  u32 currFrame;
  f32 animTime;
  bool moving;
};
