#pragma once

#include "../common.hpp"
#include <functional>
#include <raylib.h>

namespace GUI
{

enum Type
{
  PANEL,
  BUTTON,
  TEXT_BUTTON,
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

struct Item {
  Type type;
  Color color;
  Vector2 position;
  Vector2 offset;
  Vector2 dimensions;
  std::function<void()> action;

  std::string text;

  void Update( Vector2 parentPos )
  {
    this->position.x = this->offset.x + parentPos.x;
    this->position.y = this->offset.y + parentPos.y;
  }
};

struct ItemId {
  i32 index;
  Item item;
  bool operator==( const ItemId &rhs ) { return this->index == rhs.index; };
};

struct Panel {
  i32 index;
  bool floating;
  Color color;
  Vector2 position;
  Vector2 dimensions;

  Vector2 oldOffset;
  std::function<Rectangle()> update;
  std::vector<ItemId> children;

  void Update()
  {
    Rectangle updated = this->update();
    this->position = { updated.x, updated.y };
    this->dimensions = { updated.width, updated.height };
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
    {
      result = true;
    }
  }
  else if ( panel.index == context.hot )
  {
    if ( mouseWentDown )
    {
      context.active = panel.index;
    }
  }

  if ( inside )
  {
    if ( context.active == -1 )
      context.hot = panel.index;
  }

  DrawRectangleV( panel.position, panel.dimensions, panel.color );

  return result;
}

inline bool DoButton(
  Context &context,
  ItemId &itemId,
  bool inside,
  bool mouseWentUp,
  bool mouseWentDown )
{
  bool result = false;

  if ( itemId.index == context.active )
  {
    if ( mouseWentUp )
    {
      if ( itemId.index == context.hot )
        result = true;// do the button action
      context.active = -1;
    }
  }
  else if ( itemId.index == context.hot )
  {
    if ( mouseWentDown )
      context.active = itemId.index;
  }

  if ( inside )
  {
    if ( context.active == -1 )
      context.hot = itemId.index;
  }

  switch ( itemId.item.type )
  {
    case TEXT_BUTTON:
      DrawRectangleV(
        itemId.item.position,
        itemId.item.dimensions,
        itemId.item.color );

      DrawText(
        itemId.item.text.c_str(),
        itemId.item.position.x,
        itemId.item.position.y + ( 0.5 * itemId.item.dimensions.y ),
        20,
        RED );
      break;

    default:
      break;
  }


  return result;
}

inline Rectangle GetAbsoluteRectangle( Vector2 pos, Vector2 dimensions )
{
  return { pos.x, pos.y, dimensions.x, dimensions.y };
}

};// namespace GUI
