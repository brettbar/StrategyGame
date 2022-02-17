#pragma once

#include "../common.hpp"

namespace GUI
{

enum ItemType
{
  PANEL,
  BUTTON
};

enum Layout
{
  NONE,
  HORIZONTAL,
  VERTICAL
};

enum Alignment
{
  TL,
  TC,
  TR,
  ML,
  MC,
  MR,
  BL,
  BC,
  BR
};

struct Style {
  u32 margin;
  Layout layout;
  Alignment alignment;
};

struct Item {
  i32 owner;
  i32 index;
  ItemType type;
  Color color;
  Rectangle dimensions;
  Style stye;
  std::vector<Item> children;
};
// inline bool operator==( const Item &lhs, const Item &rhs )
// {
//   return lhs.id == rhs.id;
// }

struct Context {
  Item *hot;
  Item *active;
};

inline bool DoButton(
  Context &context,
  GUI::Item *item,
  bool inside,
  bool mouseWentUp,
  bool mouseWentDown )
{
  bool result = false;

  if ( item == context.active )
  {
    if ( mouseWentUp )
    {
      if ( item == context.hot ) result = true;// do the button action
      context.active = nullptr;
    }
  }
  else if ( item == context.hot )
  {
    if ( mouseWentDown ) context.active = item;
  }

  if ( inside ) context.hot = item;

  return result;
}

};// namespace GUI
