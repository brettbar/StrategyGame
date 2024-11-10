#pragma once

#include "../../shared/common.hpp"
#include "elements.hpp"
#include <raylib.h>
#include <rlgl.h>

namespace Iron {

  struct IForge {

private:
    struct Context {
      i32 hot = -1;
      i32 active = -1;
      i32 editing = -1;
    };

    union State {
      struct {
        u32 index;
      } tab_state;

      struct {
        const char *edit_text;
      } text_input_state;
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

    // void DrawDebug() {
    //   {
    //     DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
    //     DrawFPS( GetScreenWidth() - 100, 2 );
    //   }

    //   {
    //     DrawRectangle( GetScreenWidth() - 120, 0, 120, 50, BLACK );
    //     str foo = "h: " + std::to_string( context.hot ) +
    //               ", a: " + std::to_string( context.active );
    //     DrawText( foo.c_str(), GetScreenWidth() - 120, 0, 24.0f, RED );
    //   }
    // }

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

    Element *Window(rect t) {
      auto e = new Element();
      e->transform = t;
      queue.push_back( e );
      return e;
    }

    Element *Grid(
      rect t,
      u32 c,
      u32 r,
      Color color = { 0, 0, 0, 0 },
      f32 s = 1.0f
    ) {
      auto e = new Element();
      e->type = Type::Grid;
      e->id = queue.size();
      e->background = BLUE;
      e->transform = t;
      e->scale = s;
      e->background = color;
      e->t.grid = new IGrid( c, r );
      queue.push_back( e );
      return e;
    }

    Element *TextLabel(
      Element *e
    ) {
      queue.push_back( e );
      return e;
    }

    Element *TextLabel(
      rect t,
      str txt,
      Color c = { 0, 0, 0, 0 },
      f32 s = 1.0f,
      f32 border = 0.0f,
      Color border_color = WHITE,
      f32 font_size = 28.f
    ) {
      auto e = new Element();
      e->type = Type::Text;
      e->id = queue.size();
      e->background = c;
      e->transform = t;
      e->scale = s;
      e->t.text = new IText( txt, font_size );
      e->border = border;
      e->border_color = border_color;
      queue.push_back( e );
      return e;
    }

    Element *TextureLabel(
      rect t,
      hstr texture_id,
      Color color = { 0, 0, 0, 0 },
      f32 scale = 1.0f,
      f32 border = 0.0f,
      Color border_color = WHITE
    ) {
      auto e = new Element();
      e->type = Type::Texture;
      e->id = queue.size();
      e->scale = scale;
      e->background = color;
      e->transform = t;
      e->border = border;
      e->border_color = border_color;
      e->t.texture = new ITexture( texture_id );
      auto texture = Global::texture_cache[e->t.texture->texture_id]->texture;
      SetTextureFilter( texture, TEXTURE_FILTER_POINT );
      
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

    bool TextButton( rect t, str txt, Color c = { 0, 0, 0, 0 }, f32 s = 1.0f ) {
      auto e = TextLabel( t, txt, c, s );
      e->interactable = true;
      return CheckInteract( *e );
    }


    str *TextInput( rect t, str txt, Color c = { 0, 0, 0, 0 }, f32 s = 1.0f  ) {
      auto e = new Element();
      e->type = Type::TextInput;
      e->id = queue.size();
      e->background = c;
      e->transform = t;
      e->scale = s;
      e->interactable = true;
      e->stateful = true;
      e->t.text_input = new ITextInput( txt );

      str *result = nullptr;

      if ( ui_state.contains( e->id ) ) {
        e->t.text_input->editing = context.editing == e->id;
        e->t.text_input->edit_text = ui_state[e->id].text_input_state.edit_text;
      }

      if ( context.editing == -1 && CheckInteract( *e ) ) {
        e->t.text_input->editing = true;
        context.editing = e->id;
      }

      if ( e->t.text_input->editing ) {
        if ( IsKeyPressed( KEY_ENTER ) ) {
          //@todo validate input
          e->t.text_input->saved_text = e->t.text_input->edit_text;
          e->t.text_input->editing = false;
          context.editing = -1;
          result = &e->t.text_input->saved_text;

          // ui_state.erase( e->id );
        }

        if ( IsKeyPressed( KEY_BACKSPACE ) ) {
          e->t.text_input->edit_text = e->t.text_input->edit_text.substr(
            0, e->t.text_input->edit_text.size() - 1
          );

          ui_state[e->id].text_input_state.edit_text =
            e->t.text_input->edit_text.c_str();
        } else {
          int key = GetKeyPressed();
          switch ( e->t.text_input->input_type ) {
            case ITextInput::InputType::Numeric:
              // @todo is input numeric
              break;
            case ITextInput::InputType::Textual:
              //@todo is input strictly textual (like a name)
              break;
            case ITextInput::InputType::Both:
              if ( ITextInput::is_valid_input_key( key ) ) {
                e->t.text_input->edit_text += key;
              }
              break;
          }

          ui_state[e->id].text_input_state.edit_text =
            e->t.text_input->edit_text.c_str();
        }
      }

      if ( !ui_state.contains( e->id ) ) {
        ui_state[e->id] = State{
          .text_input_state =
            {
              .edit_text = e->t.text_input->edit_text.c_str(),
            }
        };
      }


      queue.push_back( e );
      return result;
    }// namespace Iron


    bool TextureButton(
      rect t,
      hstr texture_id,
      Color c = { 0, 0, 0, 0 },
      f32 scale = 1.0f
    ) {
      auto e = TextureLabel( t, texture_id, c, scale );
      e->interactable = true;
      return CheckInteract( *e );
    }
  };

  inline IForge *Forge() {
    return IForge::ForgeSingleton();
  }
};// namespace Iron
