#pragma once

#include "../common.hpp"
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
  Rectangle dimensions;

  std::string text;
};

struct ItemId {
  i32 index;
  Item item;
  bool operator==( const ItemId &rhs ) { return this->index == rhs.index; };
};

struct Panel {
  i32 index;
  Type type;
  Color color;
  Rectangle dimensions;
  std::vector<ItemId> children;
};


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
      if ( itemId.index == context.hot ) result = true;// do the button action
      context.active = -1;
    }
  }
  else if ( itemId.index == context.hot )
  {
    if ( mouseWentDown ) context.active = itemId.index;
  }

  if ( inside )
  {
    if ( context.active == -1 ) context.hot = itemId.index;
  }


  switch ( itemId.item.type )
  {
    case TEXT_BUTTON:
      DrawRectangleRec( itemId.item.dimensions, itemId.item.color );
      DrawText(
        itemId.item.text.c_str(),
        itemId.item.dimensions.x,
        itemId.item.dimensions.y + ( 0.5 * itemId.item.dimensions.height ),
        20,
        RED );
      break;
  }


  return result;
}

};// namespace GUI
