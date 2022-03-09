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

  // Element( bool e, Color c, Vector2 p, Vector2 d )
  //     : enabled( e ), color( c ), position( p ), dimensions( d )
  // {
  // }

  inline bool operator==( const Element &rhs )
  {
    return this->index == rhs.index;
  };
};

struct Item : Element {
  Type type;
  Vector2 offset;
  std::function<void()> action;

  std::string text;
  Texture2D texture;

  // Item( Element e, Type t, Vector2 o, std::function<void()> a, std::string s )
  //     : Element( e ), type( t ), offset( o ), action( a ), text( s )
  // {
  // }

  void Update( Vector2 parentPos )
  {
    this->position.x = this->offset.x + parentPos.x;
    this->position.y = this->offset.y + parentPos.y;
  }
};

struct Panel : Element {
  bool floating;

  Vector2 oldOffset;
  std::function<Rectangle()> update;
  std::vector<Item> children;


  // Panel(
  //   Element e,
  //   bool f,
  //   Vector2 o,
  //   std::function<Rectangle()> u,
  //   std::vector<Item> c )
  //     : Element( e ), floating( f ), oldOffset( o ), update( u ), children( c )
  // {
  // }

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

inline bool DoItem(
  Context &context,
  Item &item,
  bool inside,
  bool mouseWentUp,
  bool mouseWentDown )
{
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
    case TEXT_BUTTON:
      DrawRectangleV( item.position, item.dimensions, item.color );

      DrawText(
        item.text.c_str(),
        item.position.x,
        item.position.y + ( 0.5 * item.dimensions.y ),
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
