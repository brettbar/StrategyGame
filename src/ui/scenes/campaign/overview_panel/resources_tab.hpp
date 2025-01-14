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
      }
    }
  }

  inline void resources_tab() {
    CLAY(
      CLAY_ID( "ResourcesPanel" ),
      CLAY_LAYOUT( { .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } )
    ) {

      resource_rows( Resources::raw );
      resource_rows( Resources::refined );
      resource_rows( Resources::products );
    }
  }
}// namespace UI
