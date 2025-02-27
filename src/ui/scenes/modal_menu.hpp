#pragma once

#include "../../shared/common.hpp"
#include "../library/text_button.hpp"
#include "clay/clay.h"
#include <raylib.h>

namespace UI {
  enum class Action_ModalMenu {
    None,
    SaveGame,
    LoadGame,
    Settings,
    ExitMainMenu,
    ExitGame,
  };


  inline Action_ModalMenu modal_menu() {
    struct Button {
      Clay_String text;
      Action_ModalMenu action;
    };

    Button buttons[] = {
      Button{
        .text = CLAY_STRING( "Save Game" ),
        .action = Action_ModalMenu::SaveGame,
      },
      Button{
        .text = CLAY_STRING( "Load Game" ),
        .action = Action_ModalMenu::LoadGame,
      },
      Button{
        .text = CLAY_STRING( "Settings" ),
        .action = Action_ModalMenu::Settings,
      },
      Button{
        .text = CLAY_STRING( "Exit to Main Menu" ),
        .action = Action_ModalMenu::ExitMainMenu,
      },
      Button{
        .text = CLAY_STRING( "Exit Game" ),
        .action = Action_ModalMenu::ExitGame,
      },
    };
    u32 num_buttons = LEN( buttons );

    CLAY(
      {
        .id = CLAY_ID( "ModalMenu" ),
        .layout =
          {
            .sizing =
              {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_GROW(),
              },
            .padding = { 16, 16 },
            .childGap = 8,
            .childAlignment =
              {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_CENTER,
              },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          },
        .backgroundColor = { 0, 0, 0, 200 },
      },
    ) {
      for ( u32 i = 0; i < num_buttons; i++ ) {
        text_button_lrg(
          CLAY_STRING( "ModalMenu::Button" ), buttons[i].text, i
        );
      }
    }

    for ( u32 i = 0; i < num_buttons; i++ ) {
      Button button = buttons[i];
      if ( ButtonWasClicked( CLAY_STRING( "ModalMenu::Button" ), i ) ) {
        return button.action;
      }
    }

    return Action_ModalMenu::None;
  }


};// namespace UI
