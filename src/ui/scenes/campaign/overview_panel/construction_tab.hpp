#pragma once

#include "../../../../shared/common.hpp"
#include "../../../library/texture_button.hpp"
#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "../../../common.h"

namespace UI {
  inline void building_icon( Clay_String label, hstr texture_id, u32 i ) {
    vec2f dimensions = { 64, 64 };
    CLAY(
      CLAY_IDI( "BuildingIcon", i ),
      CLAY_LAYOUT( {
        .sizing =
          {
            .width = CLAY_SIZING_FIXED( dimensions.x ),
            .height = CLAY_SIZING_FIXED( dimensions.y ),
          },
      } ),
      texture_button( texture_id, dimensions )
    ) {
      auto id = Clay_GetElementIdWithIndex( CLAY_STRING( "BuildingIcon" ), i );

      if ( Clay_PointerOver( id ) ) {
        CLAY(
          CLAY_IDI( "BuildingIcon::Tooltip", i ),
          CLAY_FLOATING( {
            .attachment =
              {
                .element = CLAY_ATTACH_POINT_LEFT_TOP,
                .parent = CLAY_ATTACH_POINT_LEFT_BOTTOM,
              },
            .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
          } ),
          CLAY_RECTANGLE( { .color = { 0, 0, 0, 255 }, .cornerRadius = { 5 } }
          ),
          CLAY_LAYOUT( { .padding = { 8, 8 } } )
        ) {
          CLAY_TEXT(
            label,
            CLAY_TEXT_CONFIG( {
              .textColor = COLOR_WHITE,
              .fontId = 0,
              .fontSize = 16,
            } )
          );
        }
      }
    }
  }

  inline void construction_tab() {
    struct Building {
      Clay_String label;
      str path;
    };
    list<Building> buildings = {
      { .label = CLAY_STRING( "Farm" ), .path = "farm_icon.png" },
      { .label = CLAY_STRING( "Lumber Mill" ), .path = "lumber_mill_icon.png" },
      { .label = CLAY_STRING( "Mine" ), .path = "mine_icon.png" },
    };

    CLAY(
      CLAY_ID( "ConstructionPanel" ),
      CLAY_LAYOUT( {
        .childGap = 8,
      } )
    ) {
      for ( u32 i = 0; i < buildings.size(); i++ ) {
        Building building = buildings[i];
        building_icon( building.label, hstr{ building.path.c_str() }, i );
      }
    }
  }
};// namespace UI
