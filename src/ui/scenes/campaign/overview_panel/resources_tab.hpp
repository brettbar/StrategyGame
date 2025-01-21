#pragma once

#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "../../../../../data/resources.hpp"
#include "../../../../shared/common.hpp"
#include "../../../common.h"
#include "../../../library/texture_button.hpp"

namespace UI {

  inline void resource_icon( const char *resource, vec2f dimensions, u32 i ) {

    Clay_String cs = {
      .length = strlen( resource ),
      .chars = resource,
    };


    str file = std::string( resource );

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

  inline void resource_rows( list<const char *> resources ) {
    vec2f dimensions = { 68, 68 };
    for ( u32 i = 0; i < resources.size(); i++ ) {
      CLAY( CLAY_LAYOUT( {
        .childGap = 4,
        .layoutDirection = CLAY_LEFT_TO_RIGHT,
      } ) ) {
        resource_icon( resources[i], dimensions, i );

        for ( u32 j = 0; j < 5; j++ ) {
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
              .texture_id = hstr{ "slot.png" },
            } )
          );
        }
      }
    }
  }

  inline void resources_headers() {
    list<hstr> resource_headers = {
      "resources_header_icons.png",
      "resources_header_stockpile.png",
      "resources_header_increase.png",
      "resources_header_decrease.png",
      "resources_header_incoming.png",
      "resources_header_outgoing.png",
    };

    CLAY(
      CLAY_ID( "ResourceHeaders" ),
      CLAY_LAYOUT( {
        .sizing =
          {
            .width = CLAY_SIZING_GROW(),
            .height = CLAY_SIZING_FIXED( 36 ),
          },
        .childGap = 4,
        .layoutDirection = CLAY_LEFT_TO_RIGHT,
      } )
    ) {
      for ( u32 i = 0; i < resource_headers.size(); i++ ) {
        texture_button( resource_headers[i], { 68, 36 } );
      }
    }
  }

  inline void resources_tab() {
    CLAY( CLAY_LAYOUT( {
      .childGap = 4,
      .layoutDirection = CLAY_TOP_TO_BOTTOM,
    } ) ) {
      resources_headers();
      CLAY(
        CLAY_ID( "ResourceRows" ),
        CLAY_SCROLL( { .vertical = true } ),
        CLAY_LAYOUT( {
          .sizing =
            {
              .width = CLAY_SIZING_GROW(),
            },
          .childGap = 4,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        } )
      ) {
        resource_rows( Resources::raw );
        resource_rows( Resources::refined );
        resource_rows( Resources::products );
      }
    }
  }
}// namespace UI
