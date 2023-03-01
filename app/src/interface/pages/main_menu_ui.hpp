#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

#include "../ui_system.hpp"

namespace UI {

  inline std::vector<ptr<Element>> CreateMainMenuUI() {
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

          self.transform.x = updated_pos.x;
          self.transform.y = updated_pos.y;
        },
        {
          Create<TextButton>( {
            "main_menu_host_game",
            "Host Game",
            32,
            BLUE,
            WHITE,
            Events::ID::MainMenuHostGame,
          } ),
          Create<TextButton>( {
            "main_menu_join_game",
            "Join Game",
            32,
            BLUE,
            WHITE,
            Events::ID::MainMenuJoinGame,
          } ),
          Create<TextButton>( {
            "main_menu_start_game",
            "Start Game",
            32,
            BLUE,
            WHITE,
            Events::ID::MainMenuStartGame,
          } ),
          Create<TextButton>( {
            "main_menu_load_game",
            "Load Game",
            32,
            BLUE,
            WHITE,
            Events::ID::MainMenuLoadGame,
          } ),
          Create<TextButton>( {
            "main_menu_settings",
            "Settings",
            32,
            BLUE,
            WHITE,
            Events::ID::MainMenuSettings,
          } ),
          Create<TextButton>( {
            "main_menu_exit_game",
            "Exit Game",
            32,
            BLUE,
            WHITE,
            Events::ID::MainMenuExitGame,
          } ),
        },
      } ),
    };
  }
};// namespace UI
