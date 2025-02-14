#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../common.h"


namespace UI {

  inline void texture_button(
    Clay_String id,
    hstr texture_id,
    vec2f dimensions
  ) {
    // CLAY(
    //   Clay__AttachId( Clay__HashString( id, 0, 0 ) ),
    //   CLAY_LAYOUT( {
    //     .sizing =
    //       {
    //         .width = CLAY_SIZING_FIXED( dimensions.x ),
    //         .height = CLAY_SIZING_FIXED( dimensions.y ),
    //       },
    //   } ),
    //   CLAY_IMAGE( {
    //     .sourceDimensions =
    //       {
    //         dimensions.x,
    //         dimensions.y,
    //       },
    //     .texture_id = texture_id,
    //   } )
    // );
  }


  inline void texture_label( hstr texture_id, vec2f dimensions ) {
    CLAY( {
      .layout =
        {
          .sizing =
            {
              .width = CLAY_SIZING_FIXED( dimensions.x ),
              .height = CLAY_SIZING_FIXED( dimensions.y ),
            },
        },
      .image =
        {
          .imageData = (void *) &Global::texture_cache[texture_id]->texture,
          .sourceDimensions =
            {
              dimensions.x,
              dimensions.y,
            },
        },
    } );
  }


  inline Clay_ImageElementConfig image( hstr texture_id, vec2f dimensions ) {
    return {
      .imageData = (void *) &Global::texture_cache[texture_id]->texture,
      .sourceDimensions =
        {
          dimensions.x,
          dimensions.y,
        },
    };
  }

}// namespace UI
