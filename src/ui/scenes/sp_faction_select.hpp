#pragma once

#include "../../shared/common.hpp"
#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "clay/clay.h"

#include "../../world/managers/actor_manager.hpp"
#include "../../world/managers/faction_manager.hpp"

#include "../library/text_button.hpp"
#include <raylib.h>

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
        // Clay_String poop = CLAY_STRING( "romans" );
        const char *faction = fm->ids[i].c_str();

        auto actor = Actor::Manager::Get()->actor_from_faction_roster(
          Actor::Type::ArmyTierI, faction
        );

        Clay_String cs = (Clay_String) {
          .chars = faction,
          .length = strlen( faction ),
        };

        Color faction_color = fm->primary_colors[faction];
        Clay_Color fc = {
          .r = (f32) faction_color.r,
          .g = (f32) faction_color.g,
          .b = (f32) faction_color.b,
          .a = (f32) faction_color.a
        };


        CLAY( CLAY_LAYOUT( {
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
        } ) ) {
          RenderMenuButton( cs, i, fc );

          CLAY(
            CLAY_LAYOUT(
              { .sizing =
                  { .width = CLAY_SIZING_FIXED( 128 ),
                    .height = CLAY_SIZING_FIXED( 128 ) } }
            ),
            CLAY_IMAGE(
              { .texture_id = hstr{ ( actor.sprite_id + "_overview" ).c_str() },
                .sourceDimensions = { 128, 128 } }
            )
          ) {}
        }
      }
    }

    for ( u32 i = 0; i < num_factions; i++ ) {
      if ( ButtonWasClicked( i ) ) {
        return fm->ids[i];
      }
    }

    return "";
  }
};// namespace UI