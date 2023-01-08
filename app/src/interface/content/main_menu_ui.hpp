#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

namespace UI {

inline std::vector<entt::entity> CreateMainMenuUI() {
  return {
    Panel::CreateAbsPos(
      "main_menu",
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
          "main_menu_host_game",
          "Host Game",
          32,
          BLUE,
          WHITE,
          false,
          true,
          []() {
            printf( "Host\n" );
            Events::event_emitter.publish( Events::UIEvent{
              "main_menu_host_game",
            } );
          }
        ),
        TextButton::Create(
          "main_menu_join_game",
          "Join Game",
          32,
          BLUE,
          WHITE,
          false,
          true,
          []() {
            printf( "Join\n" );
            Events::event_emitter.publish( Events::UIEvent{
              "main_menu_join_game",
            } );
          }
        ),
        TextButton::Create(
          "main_menu_start_game",
          "Start Game",
          32,
          BLUE,
          WHITE,
          false,
          true,
          []() {
            printf( "Start\n" );
            Events::event_emitter.publish( Events::UIEvent{
              "main_menu_start_game",
            } );
          }
        ),
        TextButton::Create(
          "main_menu_load_game",
          "Load Game",
          32,
          BLUE,
          WHITE,
          false,
          true,
          []() {
            printf( "Load\n" );
            Events::event_emitter.publish( Events::UIEvent{
              "main_menu_load_game",
            } );
          }
        ),
        TextButton::Create(
          "main_menu_settings",
          "Settings",
          32,
          BLUE,
          WHITE,
          false,
          true,
          []() { printf( "Settings\n" ); }
        ),
        TextButton::Create(
          "main_menu_exit_game",
          "Exit Game",
          32,
          BLUE,
          WHITE,
          false,
          true,
          []() {
            printf( "ExitGame\n" );
            Events::event_emitter.publish( Events::UIEvent{
              "main_menu_exit_game",
            } );
          }
        ),
      }
    ),
  };
}
};// namespace UI
