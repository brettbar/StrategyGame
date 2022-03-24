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

struct Context {
  i32 hot;
  i32 active;
};

struct Element {
  i32 index;
  bool enabled;
  Color color;
  Vector2 pos;
  Vector2 dmns;

  inline bool operator==( const Element &rhs ) { return index == rhs.index; };
};

struct Item : Element {
  Type type;
  Vector2 offset;

  inline void Update( Vector2 parentPos ) {
    pos.x = offset.x + parentPos.x;
    pos.y = offset.y + parentPos.y;
  }

  inline virtual void Draw(){};
};

struct TextLabel : Item {
  std::string text;
  i32 fontSize;
  Color textColor;

  inline void Draw() override {
    DrawRectangleV( pos, dmns, color );

    DrawText(
      text.c_str(),
      pos.x,
      pos.y + ( 0.5 * dmns.y ),
      fontSize,
      textColor );
  }
};

struct TextButton : TextLabel {
  std::function<void()> action;
};

struct TextureButton : Item {
  Texture2D texture;
  std::function<void()> action;
};

struct Panel : Element {
  bool floating;
  Vector2 oldOffset;
  std::function<Rectangle()> update;
  std::vector<GUI::Item *> children;

  void Update() {
    Rectangle updated = update();
    pos = { updated.x, updated.y };
    dmns = { updated.width, updated.height };
  }
};

inline bool DoFloatingPanel(
  Context &context,
  Panel &panel,
  bool inside,
  bool mouseWentUp,
  bool mouseWentDown,
  bool mouseHeldDown ) {
  bool result = false;

  if ( panel.index == context.active ) {
    if ( !mouseWentUp && mouseHeldDown )
      result = true;
  } else if ( panel.index == context.hot ) {
    if ( mouseWentDown )
      context.active = panel.index;
  }

  if ( inside ) {
    if ( context.active == -1 )
      context.hot = panel.index;
  }

  DrawRectangleV( panel.pos, panel.dmns, panel.color );

  return result;
}

inline bool DoItem(
  Context &context,
  Item *itemPtr,
  bool inside,
  bool mouseWentUp,
  bool mouseWentDown ) {
  Item item = *itemPtr;

  if ( !item.enabled )
    return false;

  bool result = false;

  if ( item.index == context.active ) {
    if ( mouseWentUp ) {
      if ( item.index == context.hot )
        result = true;// do the button action
      context.active = -1;
    }
  } else if ( item.index == context.hot ) {
    if ( mouseWentDown )
      context.active = item.index;
  }

  if ( inside ) {
    if ( context.active == -1 )
      context.hot = item.index;
  }

  switch ( item.type ) {
    case PANEL: {
    } break;
    case TEXT_LABEL: {
      TextLabel *label = dynamic_cast<TextLabel *>( itemPtr );
      label->Draw();
    } break;
    case TEXTURE_LABEL: {
    } break;
    case TEXTURE_BUTTON: {
    } break;
    case TEXT_BUTTON: {
      TextButton *button = dynamic_cast<TextButton *>( itemPtr );
      button->Draw();
    } break;
  }

  return result;
}


inline Rectangle GetAbsoluteRectangle( Vector2 pos, Vector2 dimensions ) {
  return { pos.x, pos.y, dimensions.x, dimensions.y };
}

};// namespace GUI
