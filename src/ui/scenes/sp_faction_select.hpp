#pragma once

#include "../../shared/common.hpp"
#include "clay/clay.h"

#include "../../world/managers/actor_manager.hpp"
#include "../../world/managers/faction_manager.hpp"

#include "../library/text_button.hpp"
#include "../library/texture_button.hpp"
#include <raylib.h>

namespace UI {
  inline str faction_selection_screen() {
    auto fm = Faction::Manager::Get();
    u32 num_factions = fm->num_factions;

    CLAY( {
      .id = CLAY_ID( "SPFactionSelect" ),
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
          .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
      .backgroundColor = COLOR_BLACK,
    } ) {
      for ( u32 i = 0; i < num_factions; i++ ) {
        // Clay_String poop = CLAY_STRING( "romans" );
        const char *faction = fm->ids[i].c_str();

        auto actor = Actor::Manager::Get()->actor_from_faction_roster(
          Actor::Type::ArmyTierI, faction
        );

        Clay_String cs = Clay_String{
          .length = static_cast<int32_t>( strlen( faction ) ),
          .chars = faction,
        };

        Color faction_color = fm->primary_colors[faction];
        Clay_Color fc = {
          .r = (f32) faction_color.r,
          .g = (f32) faction_color.g,
          .b = (f32) faction_color.b,
          .a = (f32) faction_color.a
        };


        CLAY(
          { .layout =
              {
                .padding = { 16, 16 },
                .childGap = 8,
                .childAlignment =
                  {
                    .x = CLAY_ALIGN_X_CENTER,
                    .y = CLAY_ALIGN_Y_CENTER,
                  },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
              } }
        ) {
          text_button_lrg( CLAY_STRING( "FactionSelect::Button" ), cs, i, fc );

          texture_label(
            hstr{ ( actor.sprite_id + "_overview" ).c_str() }, { 128, 128 }
          );
        }
      }
    }

    for ( u32 i = 0; i < num_factions; i++ ) {
      if ( ButtonWasClicked( CLAY_STRING( "FactionSelect::Button" ), i ) ) {
        return fm->ids[i];
      }
    }

    return "";
  }
};// namespace UI
