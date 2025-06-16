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

    CLAY({
      .id = CLAY_ID("SingleplayerLobby"),
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
    }) {
      text_button_lrg(
        CLAY_STRING("SinglePlayerLobby::SelectFaction"),
        CLAY_STRING("Select Faction"),
        0
      );
      text_button_lrg(
        CLAY_STRING("SinglePlayerLobby::ReturnToMain"),
        CLAY_STRING("Return to Main"),
        1
      );
    }

    if (button_was_clicked(
          CLAY_STRING("SinglePlayerLobby::SelectFaction"), 0
        )) {
      return Action_SinglePlayerLobby::SelectFaction;
    }

    if (button_was_clicked(CLAY_STRING("SinglePlayerLobby::ReturnToMain"), 1)) {
      return Action_SinglePlayerLobby::ReturnToMain;
    }

    return Action_SinglePlayerLobby::None;
  }


};// namespace UI
