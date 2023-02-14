#pragma once

#include "../../shared/global.hpp"
#include "../../shared/signals.hpp"
#include "../ui_shared.hpp"

namespace UI {

  enum class Axis {
    Row,
    Column,
  };

  enum class Align {
    Start,
    Center,
    End,
    SpaceOut,
  };

  struct Margins {
    u32 left;
    u32 right;
    u32 top;
    u32 bottom;
  };

  struct Element {
    std::string id;
    Color background;
    bool enabled;
    rect transform;
    Margins margins;

    std::vector<Messages::ID> subscribed_messages = {};

    virtual ~Element() {}

    Element(
      std::string id,
      Color background,
      bool enabled,
      rect transform,
      Margins margins
    )
        : id( id ), background( background ), enabled( enabled ),
          transform( transform ), margins( margins ),
          subscribed_messages( {} ) {}

    Element(
      std::string id,
      Color background,
      bool enabled,
      rect transform,
      Margins margins,
      std::vector<Messages::ID> subscribed_messages
    )
        : id( id ), background( background ), enabled( enabled ),
          transform( transform ), margins( margins ),
          subscribed_messages( subscribed_messages ) {

      Messages::dispatcher.sink<Messages::UpdateEnabled>()
        .connect<&Element::ReceiveUpdateEnabled>( this );
    }

    void ReceiveUpdateEnabled( const Messages::UpdateEnabled &event ) {
      printf( "Element::ReceiveUpdateEnabled!!!\n" );

      for ( Messages::ID msg_id: subscribed_messages ) {
        printf( "%d\n", msg_id );

        if ( msg_id == event.message_id ) {
          enabled = event.on;
          break;
        }
      }
    }

    void Enable() {
      enabled = true;
    }

    void Disable() {
      enabled = false;
    }

    bool IsEnabled() {
      return enabled;
    }
  };

};// namespace UI
