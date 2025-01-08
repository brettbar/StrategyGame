#pragma once

#include "overview_panel/root.hpp"
#include "settlement_context.hpp"

namespace UI {
  inline void campaign_panels() {
    CLAY(
      CLAY_ID( "Campaign" ),
      CLAY_LAYOUT( {
        .sizing =
          {
            .width = CLAY_SIZING_GROW(),
            .height = CLAY_SIZING_GROW(),
          },
        .layoutDirection = CLAY_LEFT_TO_RIGHT,
      } )
    ) {
      overview_panel();
      settlement_context();
    }
  }
};// namespace UI
