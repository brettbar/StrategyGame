#pragma once

#include "../../shared/common.hpp"
#include "../../shared/utils.hpp"
#include "../ui_shared.hpp"
#include <raylib.h>
#include <stack>

namespace Iron {
  enum class Type {
    Grid,
    TextLabel,
    Tabs,
  };

  struct Element;

  struct IGrid {
    u32 cols;
    u32 rows;

    IGrid() = delete;
    IGrid( u32 c, u32 r ) : cols( c ), rows( r ) {}
  };

  struct ITextLabel {
    str text;
    ITextLabel() = delete;
    ITextLabel( str t ) : text( t ) {}
  };

  struct ITabs {
    u32 current_tab;
  };

  struct Element {
    Type type;
    u32 id;
    rect transform;
    Color background;
    bool stateful = false;
    bool interactable = false;

    union {
      IGrid *grid;
      ITextLabel *text_label;
      ITabs *tabs;
    } t;


    void Draw() {

      switch ( type ) {
        case Type::Grid: {
          if ( background.a > 0 ) {
            DrawRectangleRec( transform, background );
          }
          // @volatile assuming the slots always have same dimensions
          // u32 slot_width = transform.width / t.grid->cols;
          // u32 slot_height = transform.height / t.grid->rows;

          // // Draw the grid
          // for ( u32 c = 0; c < t.grid->cols; c++ ) {
          //   for ( u32 r = 0; r < t.grid->rows; r++ ) {
          //     DrawRectangleRec(
          //       {
          //         transform.x + (f32) ( c * slot_width ),
          //         transform.y + (f32) ( r * slot_height ),
          //         (f32) slot_width,
          //         (f32) slot_height,
          //       },
          //       GRAY
          //     );
          //     DrawRectangleLinesEx(
          //       {
          //         transform.x + (f32) ( c * slot_width ),
          //         transform.y + (f32) ( r * slot_height ),
          //         (f32) slot_width,
          //         (f32) slot_height,
          //       },
          //       4,
          //       BLACK
          //     );
          //   }
          // }
        } break;
        case Type::TextLabel: {
          DrawRectangleRec( transform, background );
          DrawText(
            t.text_label->text.c_str(), transform.x, transform.y, 28, WHITE
          );
        } break;
      }
    }

    rect Col( u32 col ) {
      assert( type == Type::Grid && t.grid != nullptr && col <= t.grid->cols );

      // @volatile
      u32 slot_width = transform.width / t.grid->cols;
      u32 slot_height = transform.height / t.grid->rows;

      return rect{
        transform.x + (f32) ( col * slot_width ),
        transform.y,
        (f32) slot_width,
        (f32) slot_height * t.grid->rows,
      };
    }

    rect Cols( u32 start_col, u32 end_col ) {
      assert( type == Type::Grid && t.grid != nullptr && start_col <= end_col );

      // @volatile
      u32 slot_width = transform.width / t.grid->cols;
      u32 slot_height = transform.height / t.grid->rows;
      u32 width = 0;

      for ( u32 i = start_col; i < end_col; i++ ) {
        rect col = Col( i );
        width += col.width;
      }

      return rect{
        transform.x + (f32) ( start_col * slot_width ),
        transform.y,
        (f32) width,
        (f32) slot_height * t.grid->rows,
      };
    }

    rect Slot( u32 i ) {
      assert( type == Type::Grid && t.grid != nullptr );

      auto coords = CoordsFromIndex( i, t.grid->cols );
      // @volatile
      u32 slot_width = transform.width / t.grid->cols;
      u32 slot_height = transform.height / t.grid->rows;

      return rect{
        transform.x + (f32) ( coords.x * slot_width ),
        transform.y + (f32) ( coords.y * slot_height ),
        (f32) slot_width,
        (f32) slot_height,
      };
    }

    rect Slots( u32 start_i, u32 end_i ) {
      assert( type == Type::Grid && t.grid != nullptr && start_i <= end_i );

      auto start_coords = CoordsFromIndex( start_i, t.grid->cols );
      auto end_coords = CoordsFromIndex( end_i, t.grid->cols );

      // @volatile
      u32 slot_width = transform.width / t.grid->cols;
      u32 slot_height = transform.height / t.grid->rows;

      if ( start_i == end_i ) {
        return Slot( start_i );
      }

      // Account for 0 indexing
      u32 slots_wide = 1 + end_coords.x - start_coords.x;
      u32 slots_tall = 1 + end_coords.y - end_coords.y;


      return rect{
        transform.x + (f32) ( start_coords.x * slot_width ),
        transform.y + (f32) ( start_coords.y * slot_height ),
        (f32) ( slots_wide * slot_width ),
        (f32) ( slots_tall * slot_height ),
      };
    }
  };


};// namespace Iron
