#pragma once

#include "../../shared/global.hpp"
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
    Type type;
    Color background;
    bool enabled;
    rect transform;
    Margins margins;

    Element(
      std::string id,
      Type type,
      Color background,
      bool enabled,
      rect transform,
      Margins margins
    )
        : id( id ), type( type ), background( background ), enabled( enabled ),
          transform( transform ), margins( margins ) {}

    void Enable() {
      enabled = true;
    }

    void Disable() {
      enabled = false;
    }

    bool IsEnabled() {
      return enabled;
    }

    Type Type() {
      return type;
    }
  };

};// namespace UI
