#pragma once

#include "../../../../shared/common.hpp"
#include "../../../library/texture_button.hpp"
#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "../../../../../data/resources.hpp"
#include "../../../common.h"

namespace UI {
  inline void resource_rows( list<const char *> resources ) {
    for ( u32 i = 0; i < resources.size(); i++ ) {
      Clay_String cs = {
        .length = strlen( resources[i] ),
        .chars = resources[i],
      };
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

  inline void resources_tab() {
    CLAY(
      CLAY_ID( "ResourcesPanel" ),
      CLAY_LAYOUT( { .layoutDirection = CLAY_TOP_TO_BOTTOM } )
    ) {

      resource_rows( Resources::raw );
      resource_rows( Resources::refined );
      resource_rows( Resources::products );
    }
  }
}// namespace UI
