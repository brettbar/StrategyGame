#pragma once

#include "pages/campaign/campaign_ui.hpp"
#include "pages/faction_select_menu.hpp"
#include "pages/lobby_browser.hpp"
#include "pages/lobby_ui.hpp"
#include "pages/main_menu_ui.hpp"
#include "pages/modal_menu_ui.hpp"
#include "pages/singleplayer_lobby.hpp"
#include "ui_manager.hpp"

namespace UI
{
  struct UpdateListener
  {
    void ReceiveUpdate( const InterfaceUpdate::Update &update )
    {
      for ( auto &panel: Manager()->ActivePage() )
      {
        // TODO probably add a check that if the element is disabled we dont want to fire updates,
        // UNLESS the update is dealing with enabled, in which case a disabled element should
        // still be able to know if it got re-enabled
        panel.RealUpdate( update );
      }
    }

    void Listen()
    {
      InterfaceUpdate::dispatcher.sink<InterfaceUpdate::Update>()
        .connect<&UpdateListener::ReceiveUpdate>( this );
    }
  };

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

      UpdateListener listener{};

      listener.Listen();

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
        // base.Enable();
        base.InitialEnable();
        // base.SubscribeToUpdates();
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
        Interact( base, mouseWentUp, mouseWentDown );
      }

      Manager()->debug.Resize();
      Manager()->debug.Reposition();
      Manager()->debug.UpdateChildren();

      if ( !Manager()->over_any_elem )
        Manager()->SetContextNull();
    }

    inline void Draw()
    {
      for ( Element &base: Manager()->ActivePage() )
      {
        base.Draw();
      }

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
