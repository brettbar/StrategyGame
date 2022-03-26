#pragma once

#include "../common.hpp"
#include <functional>
#include <raylib.h>

namespace GUI {

enum Type {
  PANEL,
  TEXT_LABEL,
  TEXTURE_LABEL,
  TEXT_BUTTON,
  TEXTURE_BUTTON,
};

enum Layout { NONE, HORIZONTAL, VERTICAL };

inline entt::registry uiReg;

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
  Vector2 offset;
  std::function<Rectangle()> resize;

  void Update( Vector2 parentPos ) {
    pos.x = offset.x + parentPos.x;
    pos.y = offset.y + parentPos.y;
  }

  void Resize() {
    Rectangle updated = resize();
    this->pos = { updated.x, updated.y };
    this->dmns = { updated.width, updated.height };
  }
};

struct TextLabel {
  Element elem;
  std::string text;
  i32 fontSize;
  Color textColor;

  void Draw() {
    DrawRectangleV( elem.pos, elem.dmns, elem.color );

    DrawText(
      text.c_str(),
      elem.pos.x,
      elem.pos.y + ( 0.5 * elem.dmns.y ),
      fontSize,
      textColor );
  }
};

struct TextButton {
  TextLabel label;
  std::function<void()> action;
};

struct Panel {
  Element elem;
  bool floating;
  Vector2 oldOffset;
  std::vector<entt::entity> children;

  void Draw() { DrawRectangleV( elem.pos, elem.dmns, elem.color ); }
};

inline bool DoItem(
  Context &context,
  entt::entity entity,
  bool enabled,
  bool inside,
  bool mouseWentUp,
  bool mouseWentDown ) {

  if ( !enabled )
    return false;

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

  auto &elem = uiReg.get<Element>( entity );

  switch ( elem.type ) {
    case PANEL: {
      auto &panel = uiReg.get<Panel>( entity );
      panel.elem.Resize();
      panel.Draw();
    } break;
    case TEXT_LABEL: {
      auto &label = uiReg.get<TextLabel>( entity );
      label.Draw();
    } break;
    case TEXTURE_LABEL: {
    } break;
    case TEXTURE_BUTTON: {
    } break;
    case TEXT_BUTTON: {
      auto &button = uiReg.get<TextButton>( entity );

      button.label.Draw();
    } break;
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
