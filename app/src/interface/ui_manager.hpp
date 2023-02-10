#pragma once

#include "../shared/common.hpp"
#include "../shared/signals.hpp"
#include "../shared/utils.hpp"
#include "components/panel.hpp"
#include "components/text_elements.hpp"
#include "ui_shared.hpp"

//
// TODO remove
#include "../world/systems/selection_system.hpp"


// #include "content/campaign_ui.hpp"
// #include "content/faction_select_menu.hpp"
// #include "content/lobby_browser.hpp"
// #include "content/lobby_ui.hpp"
// #include "content/main_menu_ui.hpp"
// #include "content/modal_menu_ui.hpp"


namespace UI {
  enum PageType {
    MainMenu,
    FactionSelectMenu,
    SinglePlayerLobby,
    ModalMenu,
    Campaign,
    LobbyBrowser,
    Lobby,
  };

  using Page = std::vector<entt::entity>;

  class IManager {

public:
    // TODO(??) make private
    entt::registry registry;
    std::map<std::string, entt::entity> lookup;
    std::vector<Page> pages;

    Page &ActivePage() {
      return pages[_active_page_i];
    }

    void SetScene( PageType page ) {
      _active_page_i = page;
    }

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
        if ( mouseWentDown && interactive )
          _context.active = entity;
      }

      if ( inside ) {
        // if ( _context.active == entt::null && interactive ) {
        if ( _context.active == entt::null ) {
          _context.hot = entity;

          if ( mouseWentDown && interactive )
            _context.active = entity;
        }
      }

      return result;
    }

    // TODO(rf) this shouldnt use the selection system directly
    void DrawManagerDebugInfo() {
      DrawRectangle( GetScreenWidth() - 300, 102, 200, 24.0f, BLACK );
      std::string foo = "hot: " + EntityIdToString( _context.hot );
      DrawText( foo.c_str(), GetScreenWidth() - 300, 102, 24.0f, RED );

      DrawRectangle( GetScreenWidth() - 300, 152, 200, 24.0f, BLACK );
      std::string bar = "active: " + EntityIdToString( _context.active );
      DrawText( bar.c_str(), GetScreenWidth() - 300, 152, 24.0f, RED );

      DrawRectangle( GetScreenWidth() - 300, 202, 200, 24.0f, BLACK );
      std::string selected_ent =
        "entity: " + EntityIdToString( SelectionSystem::selected_entity );
      DrawText( selected_ent.c_str(), GetScreenWidth() - 300, 202, 24.0f, RED );
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


private:
    Context _context = { entt::null, entt::null };
    u32 _active_page_i = PageType::MainMenu;


    IManager() {}
    ~IManager() {}
  };

  inline IManager *Manager() {
    return IManager::Manager();
  }

  // TODO(??) I would prefer this be in the system namespace
  // But calling System::Create everytime feels clunky
  template<typename T>
  inline entt::entity Create( T element ) {
    entt::entity entity = Manager()->registry.create();
    Manager()->registry.emplace<T>( entity, element );
    Manager()->lookup.insert_or_assign( element.ID(), entity );
    return entity;
  }


};// namespace UI
