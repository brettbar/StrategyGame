
#pragma once

#include "../common.hpp"
#include "../resource.hpp"
#include "../state.hpp"
#include "../world/systems/map/provinces.hpp"
#include <raylib.h>

namespace UI {
  enum ItemType {
    NONE,
    PANEL,
    BUTTON,
  };

  struct Item {
    i32 id;
    i32 owner;
    i32 index;
    ItemType type;
    Rectangle dimensions;
    std::vector<Item> children;
  };
  inline bool operator==(const Item &lhs, const Item &rhs) {
    return (lhs.id == rhs.id);
  }

  struct Context {
    i32 hot;
    i32 active;
  };

  inline Item *CreateId(i32, ItemType);
  inline std::vector<Item> Build();

  inline Context Context = {
    .hot = -1,
    .active = -1,
  };

  inline std::vector<Item> UserInterface();

  inline std::vector<Item> userInterface = UserInterface();

  inline std::vector<Item> UserInterface() {
    return {
      {
        .type = PANEL,
        .dimensions = {0, 0, 80, (f32) GetScreenHeight()},
        .children = {

        },
      },
    };
  }

  inline void Draw() {
    for (Item item: userInterface) {
      switch (item.type) {
        case PANEL:
          item.dimensions.height = (f32) GetScreenHeight();
          DrawRectangleRec(item.dimensions, BLACK);
          break;
        case BUTTON:
          DrawRectangleRec(item.dimensions, BLACK);
          break;
        case NONE:
          break;
        default:
          break;
      }
    }
  }

  // inline bool DoButton(Item item, bool inside, bool mouseWentUp, bool mouseWentDown) {
  //   bool result = false;

  //   if (item == Context.active) {
  //     if (mouseWentUp) {
  //       if (id == Context.hot)
  //         result = true;// do the button action
  //       Context.active = nullptr;
  //     }
  //   } else if (id == Context.hot) {
  //     if (mouseWentDown)
  //       Context.active = id;
  //   }

  //   if (inside)
  //     Context.hot = id;

  //   //draw the button

  //   return result;
  // }

  // void ::Draw() {
  //   if (Do...) {}
  // }

}// namespace UI
