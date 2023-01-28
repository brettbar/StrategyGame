#pragma once
#include "../shared/global.hpp"

namespace UI {

  struct Context {
    entt::entity hot;
    entt::entity active;
  };

  inline f32 SCALE = 2.0f;

};// namespace UI
