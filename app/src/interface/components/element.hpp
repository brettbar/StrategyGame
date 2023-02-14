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

    Element(
      std::string id,
      Color background,
      bool enabled,
      rect transform,
      Margins margins
    )
        : id( id ), background( background ), enabled( enabled ),
          transform( transform ), margins( margins ) {}

    void ReceiveUpdateEnabled( const Messages::UpdateEnabled &event ) {
      enabled = event.on;
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
