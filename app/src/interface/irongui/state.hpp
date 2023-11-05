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

public:
    Context context;
    bool over_any_elem = false;
    bool did_action_this_frame = false;

    void DrawAll() {}

    bool MouseIsOverUI() {
      return context.hot != -1 || context.active != -1 || over_any_elem;
    }

    static StateHandler *GetStateHandler() {
      static StateHandler instance;
      return &instance;
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

  inline StateHandler *State() {
    return StateHandler::GetStateHandler();
  }

};// namespace Iron
