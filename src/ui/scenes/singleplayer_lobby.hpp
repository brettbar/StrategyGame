#pragma once

#include "clay/clay.h"

#include "../library/text_button.hpp"

namespace UI {
  enum class Action_SinglePlayerLobby {
    None,
    SelectFaction,
    ReturnToMain,
  };

  inline Action_SinglePlayerLobby singleplayer_lobby() {
    struct Button {
      Clay_String text;
      Action_SinglePlayerLobby action;
    };

    CLAY(
      CLAY_ID( "SingleplayerLobby" ),
      CLAY_RECTANGLE( {
        .color = { 43, 41, 51, 255 },
      } ),
      CLAY_LAYOUT( {
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
        .sizing =
          {
            .width = CLAY_SIZING_GROW(),
            .height = CLAY_SIZING_GROW(),
          },
        .padding = { 16, 16 },
        .childGap = 8,
        .childAlignment =
          {
            .y = CLAY_ALIGN_Y_CENTER,
            .x = CLAY_ALIGN_X_CENTER,
          },
      } )
    ) {
      text_button_lrg(
        CLAY_STRING( "SinglePlayerLobby::SelectFaction" ),
        CLAY_STRING( "Select Faction" ),
        0
      );
      text_button_lrg(
        CLAY_STRING( "SinglePlayerLobby::ReturnToMain" ),
        CLAY_STRING( "Return to Main" ),
        1
      );
    }

    if ( ButtonWasClicked(
           CLAY_STRING( "SinglePlayerLobby::SelectFaction" ), 0
         ) ) {
      return Action_SinglePlayerLobby::SelectFaction;
    }

    if ( ButtonWasClicked(
           CLAY_STRING( "SinglePlayerLobby::ReturnToMain" ), 1
         ) ) {
      return Action_SinglePlayerLobby::ReturnToMain;
    }

    return Action_SinglePlayerLobby::None;
  }


};// namespace UI
