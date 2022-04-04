#pragma once

#include "../common.hpp"
#include <functional>
#include <raylib.h>

namespace GUI {

enum class Type {
  PANEL,
  TEXT_LABEL,
  TEXTURE_LABEL,
  TEXT_BUTTON,
  TEXTURE_BUTTON,
};

enum class VertAlign {
  CENTER,
  TOP,
  BOTTOM,
};

enum class HorizAlign {
  CENTER,
  LEFT,
  RIGHT,
};

enum class Dimension {
  FIXED,
  FILL
};

inline entt::registry uiReg;

struct Context {
  entt::entity hot;
  entt::entity active;
};

struct Element {
  bool enabled;
  Color color;
  Vector2 pos;
  Vector2 dmns;
  Vector2 offset;

  HorizAlign horiz_align;
  VertAlign vert_align;
  Dimension horiz_dimension;
  Dimension vert_dimension;
};

struct Panel {
  Element elem;

  std::function<Vector2()> move;
  std::vector<entt::entity> children;
};

struct TextLabel {
  Element elem;
  std::string text;
  i32 fontSize;
  Color textColor;
};

struct TextButton {
  TextLabel label;
  std::function<void()> action;
};

//struct FloatingPanel {
//  Panel panel;
//  Vector2 oldOffset;
//};

inline void Layout( f32 screenWidth, f32 screenHeight ) {
  auto panel_view = GUI::uiReg.view<GUI::Panel>();

  for (auto &panel_entity: panel_view) {
    GUI::Panel &panel = GUI::uiReg.get<GUI::Panel>(panel_entity);

    // 1. Position Panel
    switch ( panel.elem.horiz_align ) {
      case GUI::HorizAlign::LEFT:
        panel.elem.pos.x = 0;
        break;
      case GUI::HorizAlign::CENTER:
        panel.elem.pos.x = screenWidth / 2;
        break;
      case GUI::HorizAlign::RIGHT:
        panel.elem.pos.x = screenWidth;
        break;
    }

    switch ( panel.elem.vert_align ) {
      case GUI::VertAlign::TOP:
        panel.elem.pos.y = 0;
        break;
      case GUI::VertAlign::CENTER:
        panel.elem.pos.y = screenHeight / 2;
        break;
      case GUI::VertAlign::BOTTOM:
        panel.elem.pos.y = screenHeight;
        break;
    }

    // 2. Resize Panel
    if (panel.elem.horiz_dimension == GUI::Dimension::FILL)
      panel.elem.dmns.x = screenWidth;

    if (panel.elem.vert_dimension == GUI::Dimension::FILL)
      panel.elem.dmns.y = screenHeight;

    // 3. Layout Items
    for (auto &item_entity: panel.children) {

    }
  }
}

inline bool DoItem(
  Context &context,
  entt::entity entity,
  bool inside,
  bool mouseWentUp,
  bool mouseWentDown ) {
  bool result = false;

  if ( entity == context.active ) {
    if ( mouseWentUp ) {
      if ( entity == context.hot )
        result = true;// do the button action

      context.active = entt::null;
    }
  } else if ( entity == context.hot ) {
    if ( mouseWentDown )
      context.active = entity;
  }

  if ( inside ) {
    if ( context.active == entt::null )
      context.hot = entity;
  }

  return result;
}

inline Rectangle GetAbsoluteRectangle( Vector2 pos, Vector2 dimensions ) {
  return { pos.x, pos.y, dimensions.x, dimensions.y };
}

inline bool MouseWasOverUI() {
  bool inside = false;

  for ( auto &entity: GUI::uiReg.view<GUI::Element>() ) {
    GUI::Element elem = GUI::uiReg.get<GUI::Element>( entity );

    inside = CheckCollisionPointRec(
      GetMousePosition(),
      GUI::GetAbsoluteRectangle( elem.pos, elem.dmns ) );

    if ( inside )
      return true;
  }

  return inside;
}


// inline bool DoFloatingPanel(
//   Context &context,
//   Panel &panel,
//   bool inside,
//   bool mouseWentUp,
//   bool mouseWentDown,
//   bool mouseHeldDown ) {
//   bool result = false;

//   if ( panel.elem == context.active ) {
//     if ( !mouseWentUp && mouseHeldDown )
//       result = true;
//   } else if ( panel.elem == context.hot ) {
//     if ( mouseWentDown )
//       context.active = panel.elem.entity;
//   }

//   if ( inside ) {
//     if ( context.active == entt::null )
//       context.hot = panel.elem.entity;
//   }

//   DrawRectangleV( panel.elem.pos, panel.elem.dmns, panel.elem.color );

//   return result;
// }

};// namespace GUI
