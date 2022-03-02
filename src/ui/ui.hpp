
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
      .floating = false,
      .color = BLACK,
      .position = Vector2{ 0, 0 },
      .dimensions = Vector2{ 80, (f32) GetScreenHeight() },
      .update = []() -> Rectangle {
        return { 0, 0, 80, (f32) GetScreenHeight() };
      },
      .children =
        {
          GUI::ItemId{
            1,
            GUI::Item{
              .type = GUI::TEXT_BUTTON,
              .color = WHITE,
              .offset = { 5, 5 },
              .dimensions = { 60, 60 },
              .action =
                []() {
                  Events::dispatcher.trigger<Events::Event>(
                    Events::PROVINCES_SPAWN_PROVINCE );

                  Events::dispatcher.trigger<Events::Event>(
                    Events::SPAWN_DELETE_SPAWNED );
                },
              .text = "Spawn",
            },
          },
          GUI::ItemId{
            2,
            GUI::Item{
              .type = GUI::TEXT_BUTTON,
              .color = BLUE,
              .offset = { 5, 65 },
              .dimensions = { 60, 60 },
            },
          },
        },
    },

    GUI::Panel{
      .index = 3,
      .floating = false,
      .color = RED,
      .position =
        Vector2{
          (f32) ( GetScreenWidth() / 2.0f ) - 200,
          (f32) GetScreenHeight() - 200,
        },
      .dimensions = Vector2{ 400, 200 },
      .update = []() -> Rectangle {
        return {
          ( GetScreenWidth() / 2.0f ) - 200,
          (f32) GetScreenHeight() - 200,
          400,
          200 };
      },
      .children = {},
    },

    GUI::Panel{
      .index = 4,
      .floating = true,
      .color = BLUE,
      .position = Vector2{ 500, 100 },
      .dimensions = Vector2{ 200, 400 },
      .update = []() -> Rectangle {
        return { 500, 100, 200, 400 };
      },
      .children = {},
    },

  };
}

inline bool MouseWasOverUI()
{
  bool inside = false;

  for ( auto &panel: uiTree )
  {
    inside = CheckCollisionPointRec(
      GetMousePosition(),
      GUI::GetAbsoluteRectangle( panel.position, panel.dimensions ) );

    if ( inside ) return true;

    for ( auto &child: panel.children )
    {

      inside = CheckCollisionPointRec(
        GetMousePosition(),
        GUI::GetAbsoluteRectangle(
          child.item.position,
          child.item.dimensions ) );

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
    if ( panel.floating )
    {
    }
    else
    {
      panel.Update();
      DrawRectangleV( panel.position, panel.dimensions, panel.color );
    }

    for ( auto &child: panel.children )
    {
      child.item.Update( panel.position );

      bool inside = CheckCollisionPointRec(
        GetMousePosition(),
        GUI::GetAbsoluteRectangle(
          child.item.position,
          child.item.dimensions ) );

      if ( !overAnyElem ) overAnyElem = inside;


      // if ( inside ) std::cout << "Inside rect: " << child.index << "\n";

      if ( DoButton( uiContext, child, inside, mouseWentUp, mouseWentDown ) )
      {
        if ( ColorToInt( child.item.color ) == ColorToInt( WHITE ) )
          child.item.color = GREEN;
        else if ( ColorToInt( child.item.color ) == ColorToInt( GREEN ) )
          child.item.color = WHITE;

        child.item.action();
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
