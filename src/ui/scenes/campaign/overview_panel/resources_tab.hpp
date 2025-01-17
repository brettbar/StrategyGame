#pragma once

#include "../../../../shared/common.hpp"
#include "../../../library/texture_button.hpp"
#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "../../../../../data/resources.hpp"
#include "../../../common.h"

namespace UI {
  inline void resource_rows( list<const char *> resources ) {
    vec2f dimensions = { 64, 64 };
    for ( u32 i = 0; i < resources.size(); i++ ) {
      Clay_String cs = {
        .length = strlen( resources[i] ),
        .chars = resources[i],
      };


      str file = std::string( resources[i] );

      for ( u32 j = 0; j < file.length(); j++ ) {
        if ( file[j] == ' ' )
          file[j] = '_';
      }

      file += ".png";

      CLAY(
        Clay__AttachId( Clay__HashString( cs, i, 0 ) ),
        CLAY_LAYOUT( {
          .sizing =
            {
              .width = CLAY_SIZING_FIXED( dimensions.x ),
              .height = CLAY_SIZING_FIXED( dimensions.y ),
            },
        } ),
        CLAY_IMAGE( {
          .sourceDimensions =
            {
              dimensions.x,
              dimensions.y,
            },
          .texture_id = hstr{ "slot.png" },
        } )
      ) {
        CLAY(
          CLAY_LAYOUT( {
            .sizing =
              {
                .width = CLAY_SIZING_FIXED( dimensions.x ),
                .height = CLAY_SIZING_FIXED( dimensions.y ),
              },
          } ),
          CLAY_IMAGE( {
            .sourceDimensions =
              {
                dimensions.x,
                dimensions.y,
              },
            .texture_id = hstr{ file.c_str() },
          } )
        );

        CLAY_FLOATING( {} );

        auto id = Clay_GetElementIdWithIndex( cs, i );

        if ( Clay_PointerOver( id ) ) {
          CLAY(
            CLAY_IDI( "ResourceIcon::Tooltip", i ),
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
              cs,
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
  }

  inline void resources_tab() {
    CLAY(
      CLAY_ID( "ResourcesPanel" ),
      CLAY_SCROLL( { .vertical = true } ),
      CLAY_LAYOUT( { .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } )
    ) {

      resource_rows( Resources::raw );
      resource_rows( Resources::refined );
      resource_rows( Resources::products );
    }
  }
}// namespace UI
