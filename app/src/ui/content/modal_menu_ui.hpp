#pragma once

#include "../../common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

namespace UI {

inline std::vector<entt::entity> CreateModalMenuUI() {
  return {
    Panel::CreateAbsPos(
      "modal_menu",
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
        TextButton::Create(
          "modal_menu_load_game", "Load Game", 32, BLUE, WHITE, false, true
        ),
        TextButton::Create(
          "modal_menu_save_game", "Save Game", 32, BLUE, WHITE, false, true
        ),
        TextButton::Create(
          "modal_menu_settings", "Settings", 32, BLUE, WHITE, false, true
        ),
        TextButton::Create(
          "modal_menu_save_exit_main",
          "Save and Exit to Main Menu",
          32,
          BLUE,
          WHITE,
          false,
          true
        ),
        TextButton::Create(
          "modal_menu_exit_main",
          "Exit to Main Menu",
          32,
          BLUE,
          WHITE,
          false,
          true
        ),
        TextButton::Create(
          "modal_menu_exit_game", "Exit Game", 32, BLUE, WHITE, false, true
        ),
      }
    ),
  };
}
};// namespace UI
