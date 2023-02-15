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
private:
    bool enabled;

public:
    std::string id;
    Color background;
    rect transform;
    Margins margins;


    virtual ~Element() {}

    Element(
      std::string id,
      Color background,
      bool enabled,
      rect transform,
      Margins margins
    )
        : enabled( enabled ), id( id ), background( background ),
          transform( transform ), margins( margins ) {}


    virtual void Enable() {
      enabled = true;
    }

    virtual void Disable() {
      enabled = false;
    }

    virtual void Resize() {}
    virtual void Draw() {}
    virtual void SubscribeToMessages() {}
    virtual void UnsubscribeFromMessages() {}

    virtual void Interact( bool, bool ) {}

    bool IsEnabled() {
      return enabled;
    }
  };

};// namespace UI
