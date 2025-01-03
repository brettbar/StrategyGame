#pragma once

#include "overview_panel.hpp"
#include "settlement_context.hpp"

namespace UI {
  inline void campaign_panels() {
    CLAY(
      CLAY_ID( "Campaign" ),
      CLAY_LAYOUT( {
        .layoutDirection = CLAY_LEFT_TO_RIGHT,
        .sizing =
          {
            .width = CLAY_SIZING_GROW(),
            .height = CLAY_SIZING_GROW(),
          },
      } )
    ) {
      overview_panel();
      settlement_context();
    }
  }
};// namespace UI