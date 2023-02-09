#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

#include "../ui_system.hpp"

namespace UI {

  inline std::vector<entt::entity> CreateModalMenuUI() {
    return {
      Create<Panel>( {
        "modal_menu",
        BLACK,
        Axis::Column,
        Align::Start,
        Align::Start,
        true,
        []( Panel &self ) {
          vec2 updated_pos = {
            ( (f32) GetScreenWidth() / 2 ) - ( 200 * SCALE / 2.0f ),
            ( (f32) GetScreenHeight() / 2 ) - 200 * SCALE,
          };

          self.elem.transform.x = updated_pos.x;
          self.elem.transform.y = updated_pos.y;
        },
        {
          Create<TextButton>( {
            "modal_menu_load_game",
            "Load Game",
            32,
            BLUE,
            WHITE,
            false,
            {},
          } ),
          Create<TextButton>( {
            "modal_menu_save_game",
            "Save Game",
            32,
            BLUE,
            WHITE,
            false,
            {},
          } ),
          Create<TextButton>( {
            "modal_menu_settings",
            "Settings",
            32,
            BLUE,
            WHITE,
            false,
            {},
          } ),
          Create<TextButton>( {
            "modal_menu_save_exit_main",
            "Save and Exit to Main Menu",
            32,
            BLUE,
            WHITE,
            false,
            {},
          } ),
          Create<TextButton>( {
            "modal_menu_exit_main",
            "Exit to Main Menu",
            32,
            BLUE,
            WHITE,
            false,
            {},
          } ),
          Create<TextButton>( {
            "modal_menu_exit_game",
            "Exit Game",
            32,
            BLUE,
            WHITE,
            false,
            {},
          } ),
        },
      } ),
    };
  }
};// namespace UI
