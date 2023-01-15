#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

#include "../ui_system.hpp"

namespace UI {

  inline std::vector<entt::entity> CreateMainMenuUI() {
    return {
      Create<Panel>( {
        "main_menu",
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
            "main_menu_host_game",
            "Host Game",
            32,
            BLUE,
            WHITE,
            false,
            true,
          } ),
          Create<TextButton>( {
            "main_menu_join_game",
            "Join Game",
            32,
            BLUE,
            WHITE,
            false,
            true,
          } ),
          Create<TextButton>( {
            "main_menu_start_game",
            "Start Game",
            32,
            BLUE,
            WHITE,
            false,
            true,
          } ),
          Create<TextButton>( {
            "main_menu_load_game",
            "Load Game",
            32,
            BLUE,
            WHITE,
            false,
            true,
          } ),
          Create<TextButton>( {
            "main_menu_settings",
            "Settings",
            32,
            BLUE,
            WHITE,
            false,
            true,
          } ),
          Create<TextButton>( {
            "main_menu_exit_game",
            "Exit Game",
            32,
            BLUE,
            WHITE,
            false,
            true,
          } ),
        },
      } ),
    };
  }
};// namespace UI
