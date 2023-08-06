#pragma once

#include "element.hpp"
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
      // for ( auto &panel: )
      // {
      //   // TODO probably add a check that if the element is disabled we dont want to fire updates,
      //   // UNLESS the update is dealing with enabled, in which case a disabled element should
      //   // still be able to know if it got re-enabled
      //   panel->ExecuteInterfaceUpdate( update );
      // }

      Manager()->ActivePage()->ExecuteInterfaceUpdate( update );
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
        // CreateFactionSelectMenuUI(),
        // CreateSinglePlayerLobby(),
        // CreateModalMenuUI(),
        // CreateCampaignUI(),
        // CreateLobbyBrowser(),
        // CreateLobbyUI(),
      };

      // THIS iS causing a crash ??
      // for ( sptr<Element> page: Manager()->pages )
      // {
      //   page->Register();
      // }

      UpdateListener listener{};

      listener.Listen();

      // Manager()->debug.Enable();
      Manager()->SetScene( MainMenu );
      EnableContent();
    }// namespace System

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
      if ( Manager()->ActivePage()->starts_disabled )
        return;

      Manager()->ActivePage()->Enable();
    }

    inline void DisableCurrentContent()
    {
      Manager()->SetContextNull();
      Manager()->ActivePage()->Disable();
    }

    inline void UpdateOnFrame()
    {
      vec2 mousePos = GetMousePosition();
      bool mouseWentUp = IsMouseButtonReleased( 0 );
      bool mouseWentDown = IsMouseButtonPressed( 0 );
      bool mouseHeldDown = IsMouseButtonDown( 0 );
      const f32 screen_width = GetScreenWidth();
      const f32 screen_height = GetScreenHeight();

      if ( screen_width >= 3840 )
      {
        SCALE = 1.5;
      }
      else if ( screen_width >= 2560 )
      {
        SCALE = 1.25;
      }
      else
      {
        SCALE = 1.0;
      }

      InterfaceUpdate::dispatcher.update();

      Manager()->over_any_elem = false;

      Manager()->ActivePage()->ResizeRecursive();
      Manager()->ActivePage()->RepositionRecursive();
      Interact( Manager()->ActivePage(), mouseWentUp, mouseWentDown );

      // Manager()->debug.ResizeRecursive();
      // Manager()->debug.RepositionRecursive();
      // Manager()->debug.Update();

      if ( !Manager()->over_any_elem )
        Manager()->SetContextNull();
    }

    inline void Draw()
    {
      // Manager()->ActivePage()->Draw();

      draw();


      // Manager()->debug.Draw();

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
};  // namespace UI
