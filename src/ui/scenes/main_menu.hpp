#pragma once

#include "../../shared/common.hpp"
#include "../library/text_button.hpp"
#include "clay/clay.h"
#include <raylib.h>

namespace UI {
  enum class Action_MainMenu {
    None,
    StartGame,
    LoadGame,
    HostGame,
    JoinGame,
    Settings,
    ExitGame,
  };


  inline Action_MainMenu main_menu() {
    struct Button {
      Clay_String text;
      Action_MainMenu action;
    };

    Action_MainMenu action = Action_MainMenu::None;

    Button buttons[] = {
      Button{
        .text = CLAY_STRING("Start Game"), .action = Action_MainMenu::StartGame
      },
      Button{
        .text = CLAY_STRING("Load Game"),
        .action = Action_MainMenu::LoadGame,
      },
      Button{
        .text = CLAY_STRING("Host Game"),
        .action = Action_MainMenu::HostGame,
      },
      Button{
        .text = CLAY_STRING("Join Game"),
        .action = Action_MainMenu::JoinGame,
      },
      Button{
        .text = CLAY_STRING("Settings"),
        .action = Action_MainMenu::Settings,
      },
      Button{
        .text = CLAY_STRING("Exit Game"),
        .action = Action_MainMenu::ExitGame,
      },
    };
    u32 num_buttons = LEN(buttons);

    CLAY(
      {
        .id = CLAY_ID("MainMenu"),
        .layout =
          {
            .sizing =
              {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_GROW(),
              },
            .padding = {16, 16},
            .childGap = 8,
            .childAlignment =
              {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_CENTER,
              },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          },
        .backgroundColor = COLOR_BLACK,
      },
    ) {
      for (u32 i = 0; i < num_buttons; i++) {
        if (text_button_lrg(
              CLAY_STRING("MainMenu::Button"), buttons[i].text, i
            )) {
          action = buttons[i].action;
        }
      }
    }

    return action;
  }


};// namespace UI
