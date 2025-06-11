#pragma once

#include "../../common.h"
#include "../../library/text_button.hpp"

namespace UI {
  enum class Action_Minimap {
    None,
    Default,
    Terrain,
    Political,
    Resources,
  };

  inline Action_Minimap minimap() {
    struct Button {
      Clay_String text;
      Action_Minimap action;
      Clay_Color color;
    };

    Action_Minimap action = Action_Minimap::None;

    Button buttons[] = {
      {
        .text = CLAY_STRING("Default"),
        .action = Action_Minimap::Default,
        .color = COLOR_SLATE,
      },
      {
        .text = CLAY_STRING("Terrain"),
        .action = Action_Minimap::Terrain,
        .color = COLOR_GREEN,
      },
      {
        .text = CLAY_STRING("Political"),
        .action = Action_Minimap::Political,
        .color = COLOR_RED,
      },
      {
        .text = CLAY_STRING("Resources"),
        .action = Action_Minimap::Resources,
        .color = COLOR_ORANGE,
      },
    };

    u32 num_buttons = LEN(buttons);

    CLAY(CLAY_ID("Minimap")) {
      for (u32 i = 0; i < num_buttons; i++) {
        if (text_button_small(
              CLAY_STRING("MapMode::Button"),
              buttons[i].text,
              i,
              buttons[i].color
            )) {
          action = buttons[i].action;
        }
      }
    }

    return action;
  }

};// namespace UI
