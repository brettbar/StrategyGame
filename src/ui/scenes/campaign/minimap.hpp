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
    CLAY( CLAY_ID( "Minimap" ) ) {
      text_button_small(
        CLAY_STRING( "MapMode::Default" ),
        CLAY_STRING( "Default" ),
        0,
        COLOR_SLATE
      );

      text_button_small(
        CLAY_STRING( "MapMode::Terrain" ),
        CLAY_STRING( "Terrain" ),
        0,
        COLOR_GREEN
      );

      text_button_small(
        CLAY_STRING( "MapMode::Political" ),
        CLAY_STRING( "Political" ),
        0,
        COLOR_RED
      );

      text_button_small(
        CLAY_STRING( "MapMode::Resources" ),
        CLAY_STRING( "Resources" ),
        0,
        COLOR_ORANGE
      );
    }

    if ( ButtonWasClicked( CLAY_STRING( "MapMode::Default" ) ) ) {
      return Action_Minimap::Default;
    }

    if ( ButtonWasClicked( CLAY_STRING( "MapMode::Terrain" ) ) ) {
      return Action_Minimap::Terrain;
    }

    if ( ButtonWasClicked( CLAY_STRING( "MapMode::Political" ) ) ) {
      return Action_Minimap::Political;
    }

    if ( ButtonWasClicked( CLAY_STRING( "MapMode::Resources" ) ) ) {
      return Action_Minimap::Resources;
    }

    return Action_Minimap::None;
  }

};// namespace UI
