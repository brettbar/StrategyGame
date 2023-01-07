#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

namespace UI {

inline std::vector<entt::entity> CreateFactionSelectMenuUI() {
  return {
    Panel::CreateAbsPos(
      "facton_select_menu",
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
      {
        TextLabel::Create(
          "faction_select_label", "Select your faction", 32, BLACK, WHITE, false
        ),
        TextButton::Create(
          "faction_select_romans", "Roman Republic", 32, RED, WHITE, false, true
        ),
        TextButton::Create(
          "faction_select_greeks", "Greek Cities", 32, BLUE, WHITE, false, true
        ),
        TextButton::Create(
          "faction_select_celts", "Celtic Tribes", 32, GREEN, WHITE, false, true
        ),
        TextButton::Create(
          "faction_select_punics",
          "Phoenician Colonies",
          32,
          PURPLE,
          WHITE,
          false,
          true
        ),
        TextButton::Create(
          "faction_select_persians",
          "Persian Empire",
          32,
          ORANGE,
          WHITE,
          false,
          true
        ),
        TextButton::Create(
          "faction_select_scythians",
          "Scythian Horde",
          32,
          PINK,
          WHITE,
          false,
          true
        ),
        TextButton::Create(
          "faction_select_germans",
          "Germanic Federation",
          32,
          GRAY,
          WHITE,
          false,
          true
        ),


      }
    ),
  };
}
};// namespace UI
