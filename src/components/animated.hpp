#pragma once
#include "../common.hpp"

struct Animated {
  Texture2D sprite;
  Rectangle frameRec;
  u32 frameIndex;
};
