#pragma once

#include "../common.hpp"
#include <functional>
#include <raylib.h>

namespace GUI
{

enum Type
{
  PANEL,
  TEXT_LABEL,
  TEXTURE_LABEL,
  TEXT_BUTTON,
  TEXTURE_BUTTON,
};

enum Layout
{
  NONE,
  HORIZONTAL,
  VERTICAL
};

struct Context {
  i32 hot;
  i32 active;
};

struct Element {
  i32 index;
  bool enabled;
  Color color;
  Vector2 position;
  Vector2 dimensions;

  inline bool operator==( const Element &rhs ) { return index == rhs.index; };


  inline void Update(){};
  inline void Update( Vector2 ){};
};

struct Item : Element {
  Type type;
  Vector2 offset;
  std::function<void()> action;

  inline void Update( Vector2 parentPos )
  {
    position.x = offset.x + parentPos.x;
    position.y = offset.y + parentPos.y;
  }

  inline virtual void Draw(){};
};

struct TextLabel : Item {
  std::string text;
  i32 fontSize;
  Color textColor;
};

struct TextButton : TextLabel {
  inline void Draw() override
  {
    DrawRectangleV( position, dimensions, color );

    DrawText(
      text.c_str(),
      position.x,
      position.y + ( 0.5 * dimensions.y ),
      fontSize,
      textColor );
  }
};

struct TextureButton : Item {
  Texture2D texture;
};

struct Panel : Element {
  bool floating;
  Vector2 oldOffset;
  std::function<Rectangle()> update;
  std::vector<GUI::Item *> children;

  void Update()
  {
    Rectangle updated = update();
    position = { updated.x, updated.y };
    dimensions = { updated.width, updated.height };
  }
};

inline bool DoFloatingPanel(
  Context &context,
  Panel &panel,
  bool inside,
  bool mouseWentUp,
  bool mouseWentDown,
  bool mouseHeldDown )
{
  bool result = false;

  if ( panel.index == context.active )
  {
    if ( !mouseWentUp && mouseHeldDown )
      result = true;
  }
  else if ( panel.index == context.hot )
  {
    if ( mouseWentDown )
      context.active = panel.index;
  }

  if ( inside )
  {
    if ( context.active == -1 )
      context.hot = panel.index;
  }

  DrawRectangleV( panel.position, panel.dimensions, panel.color );

  return result;
}

inline bool DoItem(
  Context &context,
  Item *itemPtr,
  bool inside,
  bool mouseWentUp,
  bool mouseWentDown )
{
  Item item = *itemPtr;

  if ( !item.enabled )
    return false;

  bool result = false;

  if ( item.index == context.active )
  {
    if ( mouseWentUp )
    {
      if ( item.index == context.hot )
        result = true;// do the button action
      context.active = -1;
    }
  }
  else if ( item.index == context.hot )
  {
    if ( mouseWentDown )
      context.active = item.index;
  }

  if ( inside )
  {
    if ( context.active == -1 )
      context.hot = item.index;
  }

  switch ( item.type )
  {
    case PANEL:
    {
    }
    break;
    case TEXT_LABEL:
    {
      TextLabel *label = dynamic_cast<TextLabel *>( itemPtr );
      label->Draw();
    }
    break;
    case TEXTURE_LABEL:
    {
    }
    break;
    case TEXTURE_BUTTON:
    {
    }
    break;
    case TEXT_BUTTON:
    {
      TextButton *button = dynamic_cast<TextButton *>( itemPtr );
      button->Draw();
    }
    break;
  }

  return result;
}


inline Rectangle GetAbsoluteRectangle( Vector2 pos, Vector2 dimensions )
{
  return { pos.x, pos.y, dimensions.x, dimensions.y };
}

};// namespace GUI
