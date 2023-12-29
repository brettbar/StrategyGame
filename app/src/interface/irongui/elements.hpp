#pragma once

#include "../../shared/common.hpp"
#include "../../shared/utils.hpp"
#include "../ui_shared.hpp"
#include <raylib.h>
#include <stack>

namespace Iron {
  enum class Type {
    Grid,
    Text,
    TextInput,
    Texture,
    Tabs,
  };

  struct Element;

  struct IGrid {
    u32 cols;
    u32 rows;

    list<f32> col_weights = {};
    list<f32> row_weights = {};

    IGrid() = delete;
    IGrid( u32 c, u32 r ) : cols( c ), rows( r ) {}
  };

  struct IText {
    str text;
    IText() = delete;
    IText( str t ) : text( t ) {}
  };

  struct ITextInput {
    str text;
    bool editing;
    ITextInput() = delete;
    ITextInput( str t ) : text( t ), editing( false ) {}
  };

  struct ITexture {
    hstr texture_id;
    ITexture() = delete;
    ITexture( hstr id ) : texture_id( id ) {}
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
      IText *text;
      ITextInput *text_input;
      ITexture *texture;
      ITabs *tabs;
    } t;

    void Draw() {
      switch ( type ) {
        case Type::Grid: {
          // We don't want to draw invisible grids
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
        case Type::Text: {
          DrawRectangleRec( transform, background );
          DrawText( t.text->text.c_str(), transform.x, transform.y, 28, WHITE );
        } break;
        case Type::TextInput: {
          DrawRectangleRec( transform, background );
          DrawText( t.text->text.c_str(), transform.x, transform.y, 28, WHITE );
        } break;
        case Type::Texture:
          auto resource = Global::texture_cache[t.texture->texture_id].handle();
          Texture texture = resource.get()->texture;

          if ( background.a > 0 ) {
            DrawRectangleRec( transform, background );
          }
          DrawTexture( texture, transform.x, transform.y, WHITE );
          break;
      }
    }

    // @refactor Should probably find a way to fit all these functions into IGrid itself
    // to avoid all these asserts
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

    rect Row( u32 row ) {
      assert( type == Type::Grid && t.grid != nullptr && row <= t.grid->rows );

      // @volatile
      u32 slot_width = transform.width / t.grid->cols;
      u32 slot_height = transform.height / t.grid->rows;

      return rect{
        transform.x,
        transform.y + (f32) ( row * slot_height ),
        (f32) slot_width * t.grid->cols,
        (f32) slot_height,
      };
    }

    rect Rows( u32 start_row, u32 end_row ) {
      assert( type == Type::Grid && t.grid != nullptr && start_row <= end_row );

      // @volatile
      u32 slot_width = transform.width / t.grid->cols;
      u32 slot_height = transform.height / t.grid->rows;
      u32 height = 0;

      for ( u32 i = start_row; i < end_row; i++ ) {
        rect row = Row( i );
        height += row.height;
      }

      return rect{
        transform.x,
        transform.y + (f32) ( start_row * slot_height ),
        (f32) slot_width * t.grid->cols,
        (f32) height,
      };
    }


    rect ColsByRows( u32 start_col, u32 end_col, u32 start_row, u32 end_row ) {

      assert(
        type == Type::Grid && t.grid != nullptr && start_row <= end_row &&
        start_col <= end_col
      );

      // @volatile
      u32 slot_width = transform.width / t.grid->cols;
      u32 slot_height = transform.height / t.grid->rows;
      u32 width = 0;
      u32 height = 0;

      for ( u32 i = start_col; i < end_col; i++ ) {
        rect col = Col( i );
        width += col.width;
      }


      for ( u32 i = start_row; i < end_row; i++ ) {
        rect row = Row( i );
        height += row.height;
      }

      return rect{
        transform.x + (f32) ( start_col * slot_width ),
        transform.y + (f32) ( start_row * slot_height ),
        (f32) width,
        (f32) height,
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

    // @todo - these are defined but arent called yet
    // gonna have to determine how to do the weight multiplications.
    // Maybe default is list of 1's, so that in the standard case
    // we just multiplying by 1? Then for custom weights we pass
    // floats like 0.5, which will multiply the relevant col/row by 0.5.
    // sounds good i think but gotta figure out how to setup that default case
    // cleanly.
    void WeightCols( list<f32> col_weights ) {
      assert(
        type == Type::Grid && t.grid != nullptr &&
        col_weights.size() == t.grid->cols
      );

      t.grid->col_weights = col_weights;
    }
    void WeightRows( list<f32> row_weights ) {
      assert(
        type == Type::Grid && t.grid != nullptr &&
        row_weights.size() == t.grid->rows
      );

      t.grid->row_weights = row_weights;
    }
  };


};// namespace Iron
