#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

namespace UI {

  inline std::vector<entt::entity> CreateLobbyUI() {
    return {
      Panel::CreateDynamic(
        "lobby",
        BLACK,
        Axis::Column,
        Align::Start,
        Align::Start,
        []( Panel &self ) {
          vec2 updated_pos = {
            ( (f32) GetScreenWidth() / 2 ) - ( 200 * SCALE / 2.0f ),
            ( (f32) GetScreenHeight() / 2 ) - 200 * SCALE,
          };
          self.elem.transform.x = updated_pos.x;
          self.elem.transform.y = updated_pos.y;
        },
        {}
      ),
    };
  }
};// namespace UI
