#pragma once
#include "../shared/global.hpp"

namespace UI {

  enum class Type {
    INVALID_TYPE,
    Panel,
    StackPanel,
    TextLabel,
    TextureLabel,
    TextButton,
    TextureButton,
  };


  struct Context {
    entt::entity hot;
    entt::entity active;
  };

  inline f32 SCALE = 2.0f;

};// namespace UI
