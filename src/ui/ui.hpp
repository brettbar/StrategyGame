
#pragma once

#include "./guilib.hpp"
#include <raylib.h>

namespace UI
{

inline std::vector<GUI::Item> uiTree;
inline std::vector<GUI::Item *> uiElements;


inline GUI::Context uiContext = {
  .hot = nullptr,
  .active = nullptr,
};

inline void Init()
{
  uiTree = {
    {
      .type = GUI::PANEL,
      .color = BLACK,
      .dimensions = { 0, 0, 80, (f32) GetScreenHeight() },
      .children =
        {
          {
            .type = GUI::BUTTON,
            .color = WHITE,
            .dimensions = { 0, 0, 60, 60 },
          },
          {
            .type = GUI::BUTTON,
            .color = BLUE,
            .dimensions = { 0, 60, 60, 60 },
          },
        },
    },
  };

  uiElements = {};
  // TODO(optim), I don't need to be storing the children for
  // each element, space optimization here
  for ( auto &element: uiTree )
  {
    element.index = uiElements.size();
    uiElements.push_back( &element );

    for ( auto &child: element.children )
    {
      child.index = uiElements.size();
      uiElements.push_back( &child );
    }
  }
}

inline bool HandleMouseEvent( u32 mouseButton, Vector2 mousePos ) {}

inline void Update()
{
  Vector2 mousePos = GetMousePosition();
  const bool mouseWentUp = IsMouseButtonReleased( 0 );
  const bool mouseWentDown = IsMouseButtonPressed( 0 );

  if ( mouseWentDown ) PrintVec2( mousePos );

  for ( GUI::Item *item: uiElements )
  {
    if (
      item->type == GUI::BUTTON &&
      DoButton(
        uiContext,
        item,
        CheckCollisionPointRec( mousePos, item->dimensions ),
        mouseWentUp,
        mouseWentDown ) )
    {
      printf( "%d\n", item->index );
      item->color = RED;
    }
  }
}

inline void Draw()
{
  // @TODO(gen) maybe this goes in a fixed update loop instead of draw?
  // Update();

  DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
  DrawFPS( GetScreenWidth() - 100, 2 );

  for ( GUI::Item *item: uiElements )
  {
    DrawRectangleRec( item->dimensions, item->color );
  }
}

};// namespace UI
