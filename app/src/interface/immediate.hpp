#include "../shared/common.hpp"
#include "../shared/utils.hpp"
#include <raylib.h>

namespace Iron {
  enum class Type {
    Grid,
    TextButton,
  };

  struct Grid {
    u32 rows;
    u32 cols;
    list<rect> slots;
  };

  // Final output of this is a draw call for one frame
  struct Forge {
    Type type;
    rect transform;
    Color background;

    union {
      Grid grid;
    };

    Forge() {}
    ~Forge() {}


    inline void Grid( rect rectangle, u32 rows, u32 cols ) {
      type = Type::Grid;
      transform = rectangle;
    }

    inline rect Footprint( u32 start_slot_i, u32 slots_wide, u32 slots_tall ) {
      u32 width = 0;
      u32 height = 0;


      // starting at the slot
      // for the next slots_wide cols add up the widths
      // for the next slots_tall rows, add up the heights

      vec2u starting_coords = CoordsFromIndex( start_slot_i, grid.cols );

      for ( u32 c = starting_coords.x; c < slots_wide; c++ ) {
        width +=
          grid.slots[IndexFromCoords( c, starting_coords.y, grid.cols )].width;
      }

      for ( u32 r = starting_coords.y; r < slots_tall; r++ ) {
        height +=
          grid.slots[IndexFromCoords( starting_coords.x, r, grid.rows )].height;
      }


      return rect{
        (f32) starting_coords.x,
        (f32) starting_coords.y,
        (f32) width,
        (f32) height };
    }

    inline bool TextButton( rect rectangle, str text, Color background ) {}


    // inline void DrawTextButton(
    //   rect transform,
    //   str text,
    //   f32 font_size,
    //   Color background
    // ) {
    //   DrawRectangleRec( transform, background );
    //   DrawText( text.c_str(), transform.x, transform.y, font_size, WHITE );
    // }


    //   inline void Draw( Grid grid ) {
    //     /*
    //   0 1 1 0
    //   0 0 0 0
    //   0 0 0 0
    // */

    //     u32 slot_width = grid.transform.width / grid.cols;
    //     u32 slot_height = grid.transform.height / grid.rows;

    //     // Draw the grid
    //     for ( u32 c = 0; c < grid.cols; c++ ) {
    //       for ( u32 r = 0; r < grid.rows; r++ ) {
    //         DrawRectangleRec(
    //           {
    //             grid.transform.x + (f32) ( c * slot_width ),
    //             grid.transform.y + (f32) ( r * slot_height ),
    //             (f32) slot_width,
    //             (f32) slot_height,
    //           },
    //           GRAY
    //         );
    //         DrawRectangleLinesEx(
    //           {
    //             grid.transform.x + (f32) ( c * slot_width ),
    //             grid.transform.y + (f32) ( r * slot_height ),
    //             (f32) slot_width,
    //             (f32) slot_height,
    //           },
    //           4,
    //           BLACK
    //         );
    //       }
    //     }

    //     // for ( GridPanelElement::Slot &slot: filled_slots )
    //     // {
    //     //   slot.child->Draw();
    //     // }
    //   }
  };


};// namespace Iron
