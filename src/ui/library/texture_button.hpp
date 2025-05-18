#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../common.h"


namespace UI {
  inline Clay_ImageElementConfig image( hstr texture_id, vec2f dimensions ) {
    return {
      .imageData = (void *) &Global::texture_cache[texture_id]->texture,
      .sourceDimensions =
        {
          dimensions.x,
          dimensions.y,
        },
    };
  }

  inline void texture_button(
    Clay_String id,
    hstr texture_id,
    vec2f dimensions,
    u32 i = 0
  ) {
    CLAY( {
      .id = Clay__HashString( id, i, 0 ),
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIXED( dimensions.x * UI_SCALE ),
              .height = CLAY_SIZING_FIXED( dimensions.y * UI_SCALE ),
            },
        },
      .image = image( texture_id, dimensions ),
    } );
  }

  inline void texture_label( hstr texture_id, vec2f dimensions ) {
    CLAY( {
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIXED( dimensions.x * UI_SCALE ),
              .height = CLAY_SIZING_FIXED( dimensions.y * UI_SCALE ),
            },
        },
      .image = image( texture_id, dimensions ),
    } );
  }

  inline void texture_w_tooltip(
    Clay_String id,
    Clay_String tooltip_id,
    Clay_String label,
    hstr texture_id,
    vec2f dimensions,
    u32 i
  ) {
    auto hid = Clay__HashString( id, i, 0 );

    CLAY( {
      .id = hid,
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIXED( dimensions.x * UI_SCALE ),
              .height = CLAY_SIZING_FIXED( dimensions.y * UI_SCALE ),
            },
        },
      .image = image( texture_id, dimensions ),
    } ) {
      if ( Clay_PointerOver( hid ) ) {
        CLAY( {
          .id = Clay__HashString( tooltip_id, i, 0 ),
          .layout = { .padding = { 16, 16 } },
          .backgroundColor = COLOR_BLACK,
          .cornerRadius = { 5 },
          .floating =
            {
              .attachPoints =
                {
                  .element = CLAY_ATTACH_POINT_LEFT_TOP,
                  .parent = CLAY_ATTACH_POINT_LEFT_BOTTOM,
                },
              .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
              .attachTo = CLAY_ATTACH_TO_PARENT,
            },
        } ) {
          CLAY_TEXT(
            label,
            CLAY_TEXT_CONFIG( {
              .textColor = COLOR_WHITE,
              .fontId = 0,
              .fontSize = uint16_t( 12 * UI_SCALE ),
            } )
          );
        }
      }
    }
  }
}// namespace UI
