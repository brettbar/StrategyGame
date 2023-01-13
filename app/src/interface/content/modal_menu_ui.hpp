#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

namespace UI {

  inline std::vector<entt::entity> CreateModalMenuUI() {
    return {
      Panel::CreateAbsolute(
        "modal_menu",
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
        {
          TextButton::Create(
            "modal_menu_load_game",
            "Load Game",
            32,
            BLUE,
            WHITE,
            false,
            true,

            []() {
              printf( "Load\n" );
              Events::event_emitter.publish( Events::UIEvent{
                "modal_menu_load_game",
              } );
            }
          ),
          TextButton::Create(
            "modal_menu_save_game",
            "Save Game",
            32,
            BLUE,
            WHITE,
            false,
            true,

            []() {
              printf( "Save\n" );
              Events::event_emitter.publish( Events::UIEvent{
                "modal_menu_save_game",
              } );
            }
          ),
          TextButton::Create(
            "modal_menu_settings",
            "Settings",
            32,
            BLUE,
            WHITE,
            false,
            true,
            []() { printf( "Settings\n" ); }
          ),
          TextButton::Create(
            "modal_menu_save_exit_main",
            "Save and Exit to Main Menu",
            32,
            BLUE,
            WHITE,
            false,
            true,

            []() { printf( "SaveExitMain\n" ); }
          ),
          TextButton::Create(
            "modal_menu_exit_main",
            "Exit to Main Menu",
            32,
            BLUE,
            WHITE,
            false,
            true,
            []() {
              printf( "ExitMain\n" );
              Events::event_emitter.publish( Events::UIEvent{
                "modal_menu_exit_main" } );
            }
          ),
          TextButton::Create(
            "modal_menu_exit_game",
            "Exit Game",
            32,
            BLUE,
            WHITE,
            false,
            true,

            []() {
              printf( "ExitGame\n" );
              CloseWindow();
            }
          ),
        }
      ),
    };
  }
};// namespace UI
