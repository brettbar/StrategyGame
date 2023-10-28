#include "../shared/common.hpp"
#include "../shared/utils.hpp"
#include <raylib.h>
#include <stack>

namespace Iron {
  enum class Type {
    Grid,
    TextLabel,
  };

  struct Element;

  struct Grid {
    u32 cols;
    u32 rows;

    Grid() = delete;
    Grid( u32 c, u32 r ) : cols( c ), rows( r ) {}
  };

  struct TextLabel {
    str text;
    TextLabel() = delete;
    TextLabel( str t ) : text( t ) {}
  };


  struct Element {
    Type type;
    rect transform;
    Color background;

    union {
      Grid *grid;
      TextLabel *text_label;
    } t;

    void Draw() {

      switch ( type ) {
        case Type::Grid: {
          // @volatile
          u32 slot_width = transform.width / t.grid->cols;
          u32 slot_height = transform.height / t.grid->rows;

          // Draw the grid
          for ( u32 c = 0; c < t.grid->cols; c++ ) {
            for ( u32 r = 0; r < t.grid->rows; r++ ) {
              DrawRectangleRec(
                {
                  transform.x + (f32) ( c * slot_width ),
                  transform.y + (f32) ( r * slot_height ),
                  (f32) slot_width,
                  (f32) slot_height,
                },
                GRAY
              );
              DrawRectangleLinesEx(
                {
                  transform.x + (f32) ( c * slot_width ),
                  transform.y + (f32) ( r * slot_height ),
                  (f32) slot_width,
                  (f32) slot_height,
                },
                4,
                BLACK
              );
            }
          }

          // for ( GridPanelElement::Slot &slot: filled_slots )
          // {
          //   slot.child->Draw();
          // }
        } break;
        case Type::TextLabel: {

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
      assert( type == Type::Grid && t.grid != nullptr );

      auto coords = CoordsFromIndex( i, t.grid->cols );
      u32 slot_width = transform.width / t.grid->cols;
      u32 slot_height = transform.height / t.grid->rows;

      return rect{
        transform.x + (f32) ( coords.x * slot_width ),
        transform.y + (f32) ( coords.y * slot_height ),
        (f32) slot_width,
        (f32) slot_height,
      };
    }
  };

  struct Forge {
    list<Element *> queue;

    Element *IronGrid( rect t, u32 c, u32 r ) {
      auto e = new Element();
      e->type = Type::Grid;
      e->background = BLUE;
      e->transform = t;
      e->t.grid = new Grid( c, r );
      queue.push_back( e );
      return e;
    }

    Element *IronTextLabel( rect t, str txt ) {
      auto e = new Element();
      e->type = Type::TextLabel;
      e->background = BLUE;
      e->transform = t;
      e->t.text_label = new TextLabel( txt );
      queue.push_back( e );
      return e;
    }

    inline void Draw() {
      for ( u32 i = 0; i < queue.size(); i++ ) {
        queue[i]->Draw();
      }
    }
  };


};// namespace Iron
