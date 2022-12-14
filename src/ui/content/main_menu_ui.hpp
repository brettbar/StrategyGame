#pragma once

#include "../../common.hpp"

#include "../ui_common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

namespace UI {

inline std::vector<entt::entity> CreateMainMenuUI() {
  return {
    Panel::CreateAbsPos(
      "main_menu",
      BLACK,
      Axis::COLUMN,
      Align::START,
      Align::START,
      []() -> Vector2 {
        return {
          ( (f32) GetScreenWidth() / 2 ) - ( 200 * SCALE / 2.0f ),
          ( (f32) GetScreenHeight() / 2 ) - 200 * SCALE,
        };
      },
      {
        TextButton::Create(
          "main_menu_resume_game", "Resume Game", 32, BLUE, WHITE, false, true
        ),
        TextButton::Create(
          "main_menu_start_game", "Start Game", 32, BLUE, WHITE, false, true
        ),
        TextButton::Create(
          "main_menu_load_game", "Load Game", 32, BLUE, WHITE, false, true
        ),
        TextButton::Create(
          "main_menu_settings", "Settings", 32, BLUE, WHITE, false, true
        ),
        TextButton::Create(
          "main_menu_exit_game", "Exit Game", 32, BLUE, WHITE, false, true
        ),
      }
    ),
  };
}
};// namespace UI
