
#include "../../../../shared/common.hpp"
#include "../../../library/texture_button.hpp"
#include "clay/clay.h"

namespace UI {
  inline void construction_tab() {
    list<str> buildings = {
      "farm_icon.png",
      "lumber_mill_icon.png",
      "mine_icon.png",
    };

    CLAY(
      CLAY_ID( "ConstructionPanel" ),
      CLAY_LAYOUT( {
        .childGap = 8,
      } )
    ) {
      for ( u32 i = 0; i < buildings.size(); i++ ) {
        texture_button( hstr{ buildings[i].c_str() }, { 64, 64 } );
      }
    }
  }
};// namespace UI
