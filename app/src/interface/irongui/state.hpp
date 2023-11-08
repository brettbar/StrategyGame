#pragma once

#include "../../shared/common.hpp"
#include "elements.hpp"

namespace Iron {

  struct IForge {

private:
    struct Context {
      i32 hot = -1;
      i32 active = -1;
    };

    union State {
      struct {
        u32 index;
      } tab_state;
    };

    map<u32, State> ui_state = {};

    IForge() {}
    ~IForge() {}

    IForge( IForge const & ) = delete;
    void operator=( const IForge & ) = delete;

    list<Element *> queue;

public:
    Context context;
    bool over_any_elem = false;

    static IForge *ForgeSingleton() {
      static IForge instance;
      return &instance;
    }

    void ClearQueue() {
      queue.clear();
    }
    void ClearState() {
      ui_state.clear();
    }

    void DrawAll() {
      list<u32> stateful_ids = {};

      for ( u32 i = 0; i < queue.size(); i++ ) {
        Element *e = queue[i];

        if ( e->stateful )
          stateful_ids.push_back( e->id );

        e->Draw();
      }

      map<u32, State> new_state = {};

      for ( const u32 id: stateful_ids ) {
        if ( ui_state.contains( id ) )
          new_state[id] = ui_state[id];
      }

      ui_state = new_state;
      queue.clear();
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

    bool MouseIsOverUI() {
      return context.hot != -1 || context.active != -1 || over_any_elem;
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
      e->type = Type::Grid;
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
      e->type = Type::TextLabel;
      e->id = queue.size();
      e->background = c;
      e->transform = t;
      e->t.text_label = new ITextLabel( txt );
      queue.push_back( e );
      return e;
    }

    Element *Tabs( list<bool> tabs_clicked ) {
      auto e = new Element();
      e->type = Type::Tabs;
      e->id = queue.size();
      e->stateful = true;
      e->t.tabs = new ITabs();


      if ( !ui_state.contains( e->id ) ) {
        ui_state[e->id] = State{ .tab_state = { 0 } };
      } else {
        e->t.tabs->current_tab = ui_state[e->id].tab_state.index;
      }


      for ( u32 i = 0; i < tabs_clicked.size(); i++ ) {
        if ( tabs_clicked[i] ) {
          ui_state[e->id] = State{ .tab_state{ i } };
          e->t.tabs->current_tab = i;
        }
      }

      queue.push_back( e );
      return e;
    }

    bool TextButton( rect t, str txt, Color c ) {
      auto e = TextLabel( t, txt, c );
      e->interactable = true;
      return CheckInteract( *e );
    }
  };

  inline IForge *Forge() {
    return IForge::ForgeSingleton();
  }


};// namespace Iron
