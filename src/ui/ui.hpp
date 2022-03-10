#pragma once

#include "../common.hpp"
#include "../events.hpp"
#include "./guilib.hpp"
#include "tree.hpp"


namespace UI
{

inline void HandleFloatingPanel( GUI::Panel &, Vector2 );

// inline std::vector<std::shared_ptr<GUI::ItemId>> uiElements;

inline GUI::Context uiContext = {
  .hot = -1,
  .active = -1,
};

inline std::vector<GUI::Panel> uiTree;

inline void Init()
{
  // i32 i = -1;
  uiTree = InitTree();
}

inline bool MouseWasOverUI()
{
  bool inside = false;

  for ( auto &panel: uiTree )
  {
    inside = CheckCollisionPointRec(
      GetMousePosition(),
      GUI::GetAbsoluteRectangle( panel.position, panel.dimensions ) );

    if ( inside )
      return true;

    for ( auto &child: panel.children )
    {

      inside = CheckCollisionPointRec(
        GetMousePosition(),
        GUI::GetAbsoluteRectangle( child.position, child.dimensions ) );

      if ( inside )
        return true;
    }
  }

  return inside;
}


inline void Draw()
{
  Vector2 mousePos = GetMousePosition();
  bool mouseWentUp = IsMouseButtonReleased( 0 );
  bool mouseWentDown = IsMouseButtonPressed( 0 );
  bool mouseHeldDown = IsMouseButtonDown( 0 );
  bool overAnyElem = false;

  for ( GUI::Panel &panel: uiTree )
  {
    if ( panel.floating )
    {
      bool inside = CheckCollisionPointRec(
        GetMousePosition(),
        GUI::GetAbsoluteRectangle( panel.position, panel.dimensions ) );

      if ( !overAnyElem )
        overAnyElem = inside;

      if ( GUI::DoFloatingPanel(
             uiContext,
             panel,
             inside,
             mouseWentUp,
             mouseWentDown,
             mouseHeldDown ) )
      {
        HandleFloatingPanel( panel, mousePos );
      }
      else
      {
        panel.oldOffset = {
          panel.position.x - mousePos.x,
          panel.position.y - mousePos.y,
        };
      }
    }
    else
    {
      panel.Update();
      DrawRectangleV( panel.position, panel.dimensions, panel.color );
    }

    for ( GUI::Item &child: panel.children )
    {
      child.Update( panel.position );

      bool inside = CheckCollisionPointRec(
        GetMousePosition(),
        GUI::GetAbsoluteRectangle( child.position, child.dimensions ) );

      if ( !overAnyElem )
        overAnyElem = inside;


      if ( DoItem( uiContext, child, inside, mouseWentUp, mouseWentDown ) )
      {
        if ( ColorToInt( child.color ) == ColorToInt( WHITE ) )
          child.color = GREEN;
        else if ( ColorToInt( child.color ) == ColorToInt( GREEN ) )
          child.color = WHITE;

        child.action();
      }
    }

    DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
    DrawFPS( GetScreenWidth() - 100, 2 );

    DrawRectangle( GetScreenWidth() - 200, 102, 200, 24.0f, BLACK );
    std::string foo = "hot: " + std::to_string( uiContext.hot ) + " " +
                      "active: " + std::to_string( uiContext.active );

    DrawText( foo.c_str(), GetScreenWidth() - 200, 102, 24.0f, RED );
  }

  if ( !overAnyElem )
  {
    uiContext.hot = -1;
    uiContext.active = -1;
  }
}

inline void HandleFloatingPanel( GUI::Panel &panel, Vector2 mousePos )
{
  // printf( "DoFloatingPanel: %d\n", panel.index );
  panel.position.x = panel.oldOffset.x + mousePos.x;
  panel.position.y = panel.oldOffset.y + mousePos.y;

  if ( ( panel.position.x + panel.dimensions.x ) > GetScreenWidth() )
  {
    panel.position.x = GetScreenWidth() - panel.dimensions.x;
  }
  else if ( ( panel.position.x ) < 0 )
  {
    panel.position.x = 0;
  }

  if ( ( panel.position.y + panel.dimensions.y ) > GetScreenHeight() )
  {
    panel.position.y = GetScreenHeight() - panel.dimensions.y;
  }
  else if ( ( panel.position.y ) < 0 )
  {
    panel.position.y = 0;
  }
}

};// namespace UI
