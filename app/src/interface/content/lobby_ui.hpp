#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

namespace UI {

  inline std::vector<entt::entity> CreateLobbyUI() {
    return {
      Panel::CreateAbsPos(
        "lobby",
        BLACK,
        Axis::Column,
        Align::Start,
        Align::Start,
        []() -> Vector2 {
          return {
            ( (f32) GetScreenWidth() / 2 ) - ( 200 * SCALE / 2.0f ),
            ( (f32) GetScreenHeight() / 2 ) - 200 * SCALE,
          };
        },
        {}
      ),
    };
  }
};// namespace UI
