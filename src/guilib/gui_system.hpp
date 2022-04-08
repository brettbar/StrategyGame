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

enum class Dimension { FIXED, FILL };

inline entt::registry gui_reg;

struct Context {
  entt::entity hot;
  entt::entity active;
};

struct Element {
  Type type;
  bool enabled;
  Color color;
  Vector2 pos;
  Vector2 dmns;

  Vector2 margins;

  HorizAlign horiz_align;
  VertAlign vert_align;
  Dimension horiz_dimension;
  Dimension vert_dimension;
};

struct Panel {
  std::vector<entt::entity> children;
};

struct TextLabel {
  std::string text;
  i32 fontSize;
  Color textColor;
};

struct TextButton {
  TextLabel label;
  std::function<void()> action;
};

struct FloatingPanel {
  Panel panel;
  Vector2 oldOffset;
};

inline Rectangle RectangleFromVectors( Vector2, Vector2 );

inline void AlignElement( GUI::Element &elem, Rectangle parent ) {
  switch ( elem.horiz_align ) {
    case GUI::HorizAlign::LEFT:
      elem.pos.x = parent.x;
      break;
    case GUI::HorizAlign::CENTER:
      elem.pos.x = ( parent.width / 2 ) - ( elem.dmns.x / 2 );
      break;
    case GUI::HorizAlign::RIGHT:
      elem.pos.x = parent.width - elem.dmns.x;
      break;
  }
  elem.pos.x += elem.margins.x;

  switch ( elem.vert_align ) {
    case GUI::VertAlign::TOP:
      elem.pos.y = parent.y;
      break;
    case GUI::VertAlign::CENTER:
      elem.pos.y = ( parent.height / 2 ) - ( elem.dmns.y / 2 );
      break;
    case GUI::VertAlign::BOTTOM:
      elem.pos.y = parent.height - elem.dmns.y;
      break;
  }
  elem.pos.y += elem.margins.y;
}

inline void ResizeElement( GUI::Element &elem, f32 width, f32 height ) {
  if ( elem.horiz_dimension == GUI::Dimension::FILL )
    elem.dmns.x = width;

  if ( elem.vert_dimension == GUI::Dimension::FILL )
    elem.dmns.y = height;
}


inline void Layout( f32 screen_width, f32 screen_height ) {
  auto panel_view = GUI::gui_reg.view<GUI::Panel>();

  for ( auto &panel_entity: panel_view ) {
    GUI::Panel &panel = GUI::gui_reg.get<GUI::Panel>( panel_entity );
    GUI::Element &panel_elem = GUI::gui_reg.get<GUI::Element>( panel_entity );

    // 1. Align Panel
    AlignElement( panel_elem, { 0, 0, screen_width, screen_height } );

    // 2. Resize Panel
    ResizeElement( panel_elem, screen_width, screen_height );

    // 3. Layout Items
    for ( auto &item_entity: panel.children ) {
      GUI::Element &child = GUI::gui_reg.get<GUI::Element>( item_entity );

      AlignElement(
        child,
        RectangleFromVectors( panel_elem.pos, panel_elem.dmns ) );

      ResizeElement( child, panel_elem.dmns.x, panel_elem.dmns.y );
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

inline Rectangle RectangleFromVectors( Vector2 pos, Vector2 dimensions ) {
  return { pos.x, pos.y, dimensions.x, dimensions.y };
}

inline bool MouseWasOverUI() {
  bool inside = false;

  for ( auto &entity: GUI::gui_reg.view<GUI::Element>() ) {
    GUI::Element elem = GUI::gui_reg.get<GUI::Element>( entity );

    inside = CheckCollisionPointRec(
      GetMousePosition(),
      GUI::RectangleFromVectors( elem.pos, elem.dmns ) );

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
