#pragma once

#include "../../shared/common.hpp"
#include "elements.hpp"

namespace Iron {

  struct StateHandler {

private:
    struct Context {
      i32 hot = -1;
      i32 active = -1;
    };

    StateHandler() {}
    ~StateHandler() {}

    StateHandler( StateHandler const & ) = delete;
    void operator=( const StateHandler & ) = delete;

    list<Element *> queue;

public:
    Context context;
    bool over_any_elem = false;

    void ClearQueue() {
      queue.clear();
    }

    void DrawAll() {
      for ( const auto e: queue ) {
        e->Draw();
      }
      queue.clear();
    }

    bool MouseIsOverUI() {
      return context.hot != -1 || context.active != -1 || over_any_elem;
    }

    static StateHandler *GetStateHandler() {
      static StateHandler instance;
      return &instance;
    }

    void DrawDebug() {
      {
        DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
        DrawFPS( GetScreenWidth() - 100, 2 );
      }

      {
        DrawRectangle( GetScreenWidth() - 120, 0, 120, 50, BLACK );
        str foo = "h: " + std::to_string( context.hot ) +
                  ", a: " + std::to_string( context.active );
        DrawText( foo.c_str(), GetScreenWidth() - 120, 0, 24.0f, RED );
      }
    }


    bool CheckInteract( Element e ) {
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

    Element *Grid( rect t, u32 c, u32 r, Color color = { 0, 0, 0, 0 } ) {
      auto e = new Element();
      e->type = Type_t::Grid;
      e->id = queue.size();
      e->background = BLUE;
      e->transform = t;
      e->background = color;
      e->t.grid = new IGrid( c, r );
      queue.push_back( e );
      return e;
    }

    Element *TextLabel( rect t, str txt, Color c ) {
      auto e = new Element();
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
      return CheckInteract( *e );
    }
  };

  inline StateHandler *State() {
    return StateHandler::GetStateHandler();
  }


};// namespace Iron
