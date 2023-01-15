#pragma once

#include "../shared/common.hpp"
#include "../shared/utils.hpp"
#include "ui_shared.hpp"

// TODO remove
#include "../world/systems//selection_system.hpp"


// #include "content/campaign_ui.hpp"
// #include "content/faction_select_menu.hpp"
// #include "content/lobby_browser.hpp"
// #include "content/lobby_ui.hpp"
// #include "content/main_menu_ui.hpp"
// #include "content/modal_menu_ui.hpp"


namespace UI {
  // enum class Page {
  //   MainMenu,
  //   LobbyBrowser,
  //   Lobby,
  //   Campaign,
  //   ModalMenu,
  // };
  using Page = std::vector<entt::entity>;
  struct Scene {
    std::vector<Page> pages;
    u32 curr_page_i;
  };

  class IManager {

public:
    static IManager *Manager() {
      static IManager instance;
      return &instance;
    }
    IManager( IManager const & ) = delete;
    void operator=( const IManager & ) = delete;

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

    // TODO(rf) this shouldnt use the selection system directly
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

    bool MouseIsOverUI() {
      // This is almost sufficent, but we need to account for panels too
      // not just items that can be active?
      return _context.active != entt::null || _context.hot != entt::null;
    }

    void SetContextNull() {
      _context.hot = entt::null;
      _context.active = entt::null;
    }

    void ClearRegistry() {
      registry = {};
      registry.clear();
    }

    // TODO(??) make private
    entt::registry registry;
    std::map<std::string, entt::entity> lookup;
    std::vector<entt::entity> active_page;

private:
    Context _context = { entt::null, entt::null };


    IManager() {}
    ~IManager() {}
  };

  inline IManager *Manager() {
    return IManager::Manager();
  }
};// namespace UI


// void EnableMainMenuUI() {
//   DisableCurrentContent();
//   _content = CreateMainMenuUI();
//   EnableContent();
// }

// void EnableCampaignUI() {
//   DisableCurrentContent();
//   _content = CreateCampaignUI();
// }

// void EnableModalMenuUI() {
//   DisableCurrentContent();
//   _content = CreateModalMenuUI();
//   EnableContent();
// }

// void EnableFactionSelectMenuUI() {
//   DisableCurrentContent();
//   _content = CreateFactionSelectMenuUI();
//   EnableContent();
// }

// void EnableLobbyBrowser() {
//   DisableCurrentContent();
//   _content = CreateLobbyBrowser();
//   EnableContent();
// }

// void EnableLobby() {
//   DisableCurrentContent();
//   _content = CreateLobbyUI();
//   EnableContent();
// }


// void EnableContent() {
//   for ( entt::entity base: _content ) {
//     RecursiveToggle( base, true );
//   }
// }

// void DisableCurrentContent() {
//   SetContextNull();
//   for ( entt::entity base: _content ) {
//     RecursiveToggle( base, false );
//   }
// }
