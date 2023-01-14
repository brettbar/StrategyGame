#pragma once

#include "../shared/common.hpp"
#include "ui_shared.hpp"

#include "content/campaign_ui.hpp"
#include "content/faction_select_menu.hpp"
#include "content/lobby_browser.hpp"
#include "content/lobby_ui.hpp"
#include "content/main_menu_ui.hpp"
#include "content/modal_menu_ui.hpp"


namespace UI {

  class IManager {

public:
    static IManager *Manager() {
      static IManager instance;
      return &instance;
    }
    IManager( IManager const & ) = delete;
    void operator=( const IManager & ) = delete;

    std::vector<entt::entity> CurrentContent() {
      return _content;
    }

    bool DoInteraction(
      entt::entity entity,
      bool inside,
      bool interactive,
      bool mouseWentUp,
      bool mouseWentDown
    ) {
      bool result = false;

      if ( entity == _context.active ) {
        if ( mouseWentUp ) {
          if ( entity == _context.hot )
            result = true;// do the button action

          _context.active = entt::null;
        }
      }
      else if ( entity == _context.hot ) {
        // if ( mouseWentDown && interactive )
        if ( mouseWentDown && interactive )
          _context.active = entity;
      }

      if ( inside ) {
        if ( _context.active == entt::null && interactive ) {
          _context.hot = entity;
          if ( mouseWentDown && interactive )
            _context.active = entity;
        }
      }

      return result;
    }

    bool MouseIsOverUI() {
      // This is almost sufficent, but we need to account for panels too
      // not just items that can be active?
      return _context.active != entt::null || _context.hot != entt::null;
    }

    void SetContextNull() {
      _context.hot = entt::null;
      _context.active = entt::null;
    }

    void EnableMainMenuUI() {
      DisableCurrentContent();
      _content = CreateMainMenuUI();
      EnableContent();
    }

    void EnableCampaignUI() {
      DisableCurrentContent();
      _content = CreateCampaignUI();

      Global::world.on_construct<Selected::Component>()
        .connect<&IManager::ListenForSelect>();
      Global::world.on_destroy<Selected::Component>()
        .connect<&IManager::ListenForDeselect>();
    }

    void EnableModalMenuUI() {
      DisableCurrentContent();
      _content = CreateModalMenuUI();
      EnableContent();
    }

    void EnableFactionSelectMenuUI() {
      DisableCurrentContent();
      _content = CreateFactionSelectMenuUI();
      EnableContent();
    }

    void EnableLobbyBrowser() {
      DisableCurrentContent();
      _content = CreateLobbyBrowser();
      EnableContent();
    }

    void EnableLobby() {
      DisableCurrentContent();
      _content = CreateLobbyUI();
      EnableContent();
    }


    void EnableContent() {
      for ( entt::entity base: _content ) {
        RecursiveToggle( base, true );
      }
    }

    void DisableCurrentContent() {
      SetContextNull();
      for ( entt::entity base: _content ) {
        RecursiveToggle( base, false );
      }
    }

    void DrawManagerDebugInfo() {
      DrawRectangle( GetScreenWidth() - 600, 102, 200, 24.0f, BLACK );
      std::string foo = "hot: " + EntityIdToString( _context.hot );
      DrawText( foo.c_str(), GetScreenWidth() - 600, 102, 24.0f, RED );

      DrawRectangle( GetScreenWidth() - 600, 152, 200, 24.0f, BLACK );
      std::string bar = "active: " + EntityIdToString( _context.active );
      DrawText( bar.c_str(), GetScreenWidth() - 600, 152, 24.0f, RED );

      DrawRectangle( GetScreenWidth() - 600, 202, 200, 24.0f, BLACK );
      std::string selected_ent =
        "entity: " + EntityIdToString( SelectionSystem::selected_entity );
      DrawText( selected_ent.c_str(), GetScreenWidth() - 600, 202, 24.0f, RED );
    }

private:
    enum class Page {
      MainMenu,
      LobbyBrowser,
      Lobby,
      Campaign,
      ModalMenu,
    };

    Context _context = { entt::null, entt::null };

    std::vector<entt::entity> _content;

    IManager() {}
    ~IManager() {}

    void ListenForSelect( entt::registry &game_reg, entt::entity entity ) {
      printf( "SelectListener?\n" );
      if ( game_reg.all_of<Province::Component>( entity ) ) {
        // Enabled the context panel
        auto &context = lookup.at( "settlement_context_panel" );
        RecursiveToggle( context, true );
        // ToggleElem( context, true );
        // UI::Panel context_panel = Get<UI::Panel>( context );

        // // Enable the tab group
        // entt::entity tab_grp = context_panel.children[0];
        // RecursiveToggle( tab_grp, true );

        // // Enable the content panel
        // entt::entity content = context_panel.children[1];
        // RecursiveToggle( content, true );
        // UI::StackPanel content_panel = Get<UI::StackPanel>( content );
      }
      else if ( game_reg.all_of<Actor::Component>( entity ) ) {
        auto actor = game_reg.get<Actor::Component>( entity );

        auto context_panel = lookup.at( "actor_context_panel" );
        RecursiveToggle( context_panel, true );
      }
    }

    void ListenForDeselect() {
      printf( "DeSelectListener?\n" );
      auto context_panel = lookup.at( "settlement_context_panel" );
      RecursiveToggle( context_panel, false );
      context_panel = lookup.at( "actor_context_panel" );
      RecursiveToggle( context_panel, false );
      SetContextNull();
    }
  };

  inline IManager *Manager() {
    return IManager::Manager();
  }
};// namespace UI
