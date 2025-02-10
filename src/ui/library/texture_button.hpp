#pragma once

#include "../../shared/common.hpp"
#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "clay/clay.h"

inline void texture_button(
  Clay_String id,
  hstr texture_id,
  vec2f dimensions
) {
  CLAY(
    Clay__AttachId( Clay__HashString( id, 0, 0 ) ),
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
      .texture_id = texture_id,
    } )
  );
}

inline void texture_label( hstr id, vec2f dimensions ) {
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
      .texture_id = id,
    } )
  );
}
