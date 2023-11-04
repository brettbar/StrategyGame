#pragma once

#include "../shared/common.hpp"
#include "../shared/utils.hpp"
#include "ui_shared.hpp"
#include <raylib.h>
#include <stack>

namespace Iron {
  enum class Type_t {
    Grid,
    TextLabel,
  };


  struct IElement;

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

  struct IElement {
    Type_t type;
    u32 id;
    rect transform;
    Color background;
    bool interactable = false;

    union {
      IGrid *grid;
      ITextLabel *text_label;
    } t;


    void Draw() {

      switch ( type ) {
        case Type_t::Grid: {


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

          // for ( GridPanelElement::Slot &slot: filled_slots )
          // {
          //   slot.child->Draw();
          // }
        } break;
        case Type_t::TextLabel: {

          DrawRectangleRec( transform, background );
          DrawText(
            t.text_label->text.c_str(), transform.x, transform.y, 28, WHITE
          );
        } break;
      }
      /*
      0 1 1 0
      0 0 0 0
      0 0 0 0
    */
    }

    rect Slot( u32 i ) {
      assert( type == Type_t::Grid && t.grid != nullptr );

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
      assert( type == Type_t::Grid && t.grid != nullptr && start_i <= end_i );

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


  struct Foreman {

private:
    struct Context {
      i32 hot = -1;
      i32 active = -1;
    };

    Foreman() {}
    ~Foreman() {}

    Foreman( Foreman const & ) = delete;
    void operator=( const Foreman & ) = delete;

public:
    Context context;
    bool over_any_elem = false;
    bool did_action_this_frame = false;

    bool MouseIsOverUI() {
      return context.hot != -1 || context.active != -1 || over_any_elem;
    }

    static Foreman *GetWatcher() {
      static Foreman instance;
      return &instance;
    }

    bool CheckInteract( IElement e ) {
      vec2f mouse_pos = GetMousePosition();
      bool mouse_went_up = IsMouseButtonReleased( 0 );
      bool mouse_went_down = IsMouseButtonPressed( 0 );
      // bool mouse_held_down = IsMouseButtonDown( 0 );


      bool inside = CheckCollisionPointRec( mouse_pos, e.transform );

      if ( !inside )
        return false;

      if ( !over_any_elem && e.interactable )
        over_any_elem = inside;

      bool result = false;

      if ( e.id == context.active ) {
        if ( mouse_went_up ) {
          if ( e.id == context.hot ) {
            result = true;// do the button action
            did_action_this_frame = true;
          }


          context.active = -1;
        }
      } else if ( e.id == context.hot && e.interactable ) {
        if ( mouse_went_down )
          context.active = e.id;
      }

      if ( inside ) {
        if ( context.active == -1 && e.interactable ) {
          context.hot = e.id;

          if ( mouse_went_down )
            context.active = e.id;
        }
      }

      return result;
    }
  };

  inline Foreman *Watcher() {
    return Foreman::GetWatcher();
  }

  struct Forge {
    list<IElement *> queue;

    Forge() {}
    ~Forge() {
      queue.clear();
    }


    IElement *Grid( rect t, u32 c, u32 r ) {
      auto e = new IElement();
      e->type = Type_t::Grid;
      e->id = queue.size();
      e->background = BLUE;
      e->transform = t;
      e->t.grid = new IGrid( c, r );
      queue.push_back( e );
      return e;
    }

    IElement *TextLabel( rect t, str txt, Color c ) {
      auto e = new IElement();
      e->type = Type_t::TextLabel;
      e->id = queue.size();
      e->background = c;
      e->transform = t;
      e->t.text_label = new ITextLabel( txt );
      queue.push_back( e );
      return e;
    }

    bool TextButton( rect t, str txt, Color c ) {
      auto e = TextLabel( t, txt, c );
      e->interactable = true;
      return Watcher()->context.active == e->id;
    }

    inline void Draw() {
      for ( u32 i = 0; i < queue.size(); i++ ) {
        Watcher()->CheckInteract( *queue[i] );
        queue[i]->Draw();
      }
    }
  };


};// namespace Iron
