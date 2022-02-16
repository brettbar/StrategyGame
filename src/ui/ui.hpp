
#pragma once

#include "../common.hpp"
#include "../resource.hpp"
#include "../state.hpp"
#include "../world/systems/map/provinces.hpp"
#include <raylib.h>

namespace UI {
  enum ItemType {
    PANEL,
    BUTTON,
  };

  enum Layout {
    NONE,
    HORIZONTAL,
    VERTICAL
  };

  enum Alignment {
    TL,
    TC,
    TR,
    ML,
    MC,
    MR,
    BL,
    BC,
    BR,
  };

  struct Style {
    u32 margin;

    Layout layout;
    Alignment alignment;
  };

  struct Item {
    i32 id;
    i32 owner;
    i32 index;
    ItemType type;
    Color color;
    Rectangle dimensions;
    Style style;
    std::vector<Item> children;
  };
  inline bool operator==(const Item &lhs, const Item &rhs) {
    return (lhs.id == rhs.id);
  }

  struct Context {
    i32 hot;
    i32 active;
  };

  struct MouseEvent {
    u32 mouseButton;
  };


  inline Item *CreateId(i32, ItemType);

  inline Context Context = {
    .hot = -1,
    .active = -1,
  };

  inline std::vector<Item> userInterface = {
    {
      .id = 0,
      .type = PANEL,
      .color = BLACK,
      .dimensions = {0, 0, 80, (f32) GetScreenHeight()},
      .children = {
        {
          .id = 1,
          .type = BUTTON,
          .color = WHITE,
          .dimensions = {0, 0, 60, 60},
        },
        {
          .id = 2,
          .type = BUTTON,
          .color = BLUE,
          .dimensions = {0, 60, 60, 60},
        },
      },
    },
  };

  inline bool DoButton(Item item, bool inside, bool mouseWentUp, bool mouseWentDown) {
    bool result = false;

    if (item.id == Context.active) {
      if (mouseWentUp) {
        if (item.id == Context.hot)
          result = true;// do the button action
        Context.active = -1;
      }
    } else if (item.id == Context.hot) {
      if (mouseWentDown)
        Context.active = item.id;
    }

    if (inside)
      Context.hot = item.id;

    //draw the button
    //DrawRectangleRec(item.dimensions, item.color);

    return result;
  }

  inline bool HandleMouseEvent(u32 mouseButton, Vector2 mousePos) {
    for (Item &item: userInterface) {

      if (mouseButton == 0)
        if (DoButton(item, CheckCollisionPointRec(mousePos, item.dimensions), false, true)) {
          item.color = RED;
          return true;
        }
    }
    return false;
  }

  inline void Draw() {
    for (Item item: userInterface) {
      DrawRectangleRec(item.dimensions, item.color);
      for (Item child: item.children) {
        DrawRectangleRec(item.dimensions, item.color);
      }
    }
  }


}// namespace UI
