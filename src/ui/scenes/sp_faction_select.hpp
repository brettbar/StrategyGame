#pragma once

#include "../../shared/common.hpp"
#include "clay/clay.h"

#include "../../world/managers/faction_manager.hpp"

#include "../library/text_button.hpp"

namespace UI {
  inline str faction_settlement_screen() {
    auto fm = Faction::Manager::Get();
    u32 num_factions = fm->num_factions;

    CLAY(
      CLAY_ID( "SPFactionSelect" ),
      CLAY_RECTANGLE( { .color = { 0, 0, 0, 255 } } ),
      CLAY_LAYOUT( {
        .layoutDirection = CLAY_LEFT_TO_RIGHT,
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
      for ( u32 i = 0; i < num_factions; i++ ) {
        str faction = fm->ids[i];
        const char *faction_cstr = faction.c_str();

        u32 len = strlen( faction_cstr ) + 1;
        char fixedArray[len];
        strncpy( fixedArray, faction_cstr, len - 1 );
        fixedArray[len - 1] = '\0';

        // Clay_String cs = (Clay_String) {
        //   .chars = faction_cstr,
        //   .length = strlen( faction_cstr ),
        // };

        Clay_String poop = CLAY_STRING( "romans" );

        Clay_String cs = (Clay_String) {
          .chars = fixedArray,
          .length = len - 1,
        };

        RenderMenuButton( cs, i );
      }
    }

    return "";
  }
};// namespace UI