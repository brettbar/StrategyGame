#pragma once

#include "../../shared/common.hpp"
#define CLAY_EXTEND_CONFIG_IMAGE hstr texture_id;
#include "clay/clay.h"

inline void TextureButton( hstr id, vec2f dimensions ) {
  CLAY(
    CLAY_LAYOUT(
      { .sizing =
          { .width = CLAY_SIZING_FIXED( dimensions.x ),
            .height = CLAY_SIZING_FIXED( dimensions.y ) } }
    ),
    CLAY_IMAGE( {
      .sourceDimensions =
        {
          dimensions.x,
          dimensions.y,
        },
      .texture_id = id,
    } )
  ) {}
}
