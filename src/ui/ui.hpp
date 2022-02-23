
#pragma once

#include "../events.hpp"
#include "./guilib.hpp"
#include <raylib.h>


namespace UI
{

inline std::vector<GUI::Panel> uiTree;
// inline std::vector<std::shared_ptr<GUI::ItemId>> uiElements;

inline GUI::Context uiContext = {
  .hot = -1,
  .active = -1,
};

inline void Init()
{
  uiTree = {
    GUI::Panel{
      .index = 0,
      .type = GUI::PANEL,
      .color = BLACK,
      .dimensions = { 0, 0, 80, (f32) GetScreenHeight() },
      .children =
        {
          GUI::ItemId{
            .index = 1,
            .item =
              GUI::Item{
                .type = GUI::TEXT_BUTTON,
                .color = WHITE,
                .dimensions = { 5, 5, 60, 60 },
                .text = "Spawn",
              },
          },
          GUI::ItemId{
            .index = 2,
            .item =
              {
                .type = GUI::BUTTON,
                .color = BLUE,
                .dimensions = { 5, 65, 60, 60 },
              },
          },
        },
    },
  };
}

inline bool MouseWasOverUI()
{
  bool inside = false;

  for ( auto &panel: uiTree )
  {
    inside = CheckCollisionPointRec( GetMousePosition(), panel.dimensions );

    if ( inside ) return true;

    for ( auto &child: panel.children )
    {

      inside =
        CheckCollisionPointRec( GetMousePosition(), child.item.dimensions );

      if ( inside ) return true;
    }
  }

  return inside;
}


inline void Draw()
{
  // Vector2 mousePos = GetMousePosition();
  bool mouseWentUp = IsMouseButtonReleased( 0 );
  bool mouseWentDown = IsMouseButtonPressed( 0 );
  bool overAnyElem = false;

  for ( auto &panel: uiTree )
  {
    DrawRectangleRec( panel.dimensions, panel.color );

    for ( auto &child: panel.children )
    {

      bool inside =
        CheckCollisionPointRec( GetMousePosition(), child.item.dimensions );

      if ( !overAnyElem ) overAnyElem = inside;


      // if ( inside ) std::cout << "Inside rect: " << child.index << "\n";

      if ( DoButton( uiContext, child, inside, mouseWentUp, mouseWentDown ) )
      {
        child.item.color = GREEN;
        Events::dispatcher.trigger<Events::Event>(
          Events::PROVINCES_SPAWN_PROVINCE );
        Events::dispatcher.trigger<Events::Event>(
          Events::SPAWN_DELETE_SPAWNED );
      }
    }
  }

  if ( !overAnyElem )
  {
    uiContext.hot = -1;
    uiContext.active = -1;
  }
}

// inline void Draw()
// {
//   // @TODO(gen) maybe this goes in a fixed update loop instead of draw?
//   // Update();

//   DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
//   DrawFPS( GetScreenWidth() - 100, 2 );

//   for ( std::shared_ptr<GUI::ItemId> id: uiElements )
//   {
//     DrawRectangleRec( id->item.dimensions, id->item.color );
//   }
// }
};// namespace UI
