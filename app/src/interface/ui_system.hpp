#pragma once

#include "pages/campaign_ui.hpp"
#include "pages/faction_select_menu.hpp"
#include "pages/lobby_browser.hpp"
#include "pages/lobby_ui.hpp"
#include "pages/main_menu_ui.hpp"
#include "pages/modal_menu_ui.hpp"
#include "pages/singleplayer_lobby.hpp"
#include "ui_manager.hpp"

namespace UI
{

  namespace System
  {
    void EnableContent();
    void DisableCurrentContent();

    void Draw();

    inline void Init()
    {
      Manager()->pages = {
        CreateMainMenuUI(),
        CreateFactionSelectMenuUI(),
        CreateSinglePlayerLobby(),
        CreateModalMenuUI(),
        CreateCampaignUI(),
        CreateLobbyBrowser(),
        CreateLobbyUI(),
      };

      for ( Page &page: Manager()->pages )
      {
        for ( Element &panel: page )
        {
          panel.Register();
        }
      }

      Manager()->debug.Enable();
      Manager()->SetScene( MainMenu );
      EnableContent();
    }

    inline void SwitchPage( PageType page )
    {
      DisableCurrentContent();
      Manager()->SetScene( page );
      EnableContent();
    }

    inline void EnableCampaignUI()
    {
      DisableCurrentContent();
      Manager()->SetScene( Campaign );
    }

    inline void EnableContent()
    {
      for ( Element &base: Manager()->ActivePage() )
      {
        base.Enable();
        base.SubscribeToUpdates();
      }
    }

    inline void DisableCurrentContent()
    {
      Manager()->SetContextNull();

      for ( Element &base: Manager()->ActivePage() )
      {
        base.Disable();
      }
    }

    inline void UpdateOnFrame()
    {
      vec2 mousePos = GetMousePosition();
      bool mouseWentUp = IsMouseButtonReleased( 0 );
      bool mouseWentDown = IsMouseButtonPressed( 0 );
      bool mouseHeldDown = IsMouseButtonDown( 0 );
      const f32 screen_width = GetScreenWidth();
      const f32 screen_height = GetScreenHeight();

      if ( screen_width > 1920 )
      {
        SCALE = 2.0;
      }
      else if ( screen_width > 2560 )
      {
        SCALE = 3.0;
      }
      else if ( screen_width >= 3840 )
      {
        SCALE = 4.0;
      }

      InterfaceUpdate::dispatcher.update();

      Manager()->over_any_elem = false;

      for ( Element &base: Manager()->ActivePage() )
      {
        base.Resize();
        base.Reposition();
        base.Interact( mouseWentUp, mouseWentDown );

        // Panel & panel = Get<Panel>( base );
        // panel->Resize();
        // panel->Reposition();
        // panel->Interact( mouseWentUp, mouseWentDown );
      }


      Manager()->debug.Resize();
      Manager()->debug.Reposition();
      Manager()->debug.Update();

      if ( !Manager()->over_any_elem )
        Manager()->SetContextNull();
    }

    inline void Draw()
    {
      for ( Element &base: Manager()->ActivePage() )
      {
        base.Draw();
      }

      // DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
      // DrawFPS( GetScreenWidth() - 100, 2 );

      // Manager()->DrawManagerDebugInfo();
      Manager()->debug.Draw();

      DrawRectangle( GetScreenWidth() - 600, 252, 200, 24.0f, BLACK );
      DrawText(
        std::to_string( Global::state.timeScale ).c_str(),
        GetScreenWidth() - 600,
        252,
        24.0f,
        WHITE
      );
    }

  };// namespace System

};// namespace UI
