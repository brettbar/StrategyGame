#pragma once

#include "../shared/common.hpp"
#include "../shared/signals.hpp"
#include "../shared/utils.hpp"
#include "components/panel.hpp"
#include "components/text_elements.hpp"
#include "ui_shared.hpp"

// TODO remove
#include "../world/systems/selection_system.hpp"

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

  using Page = std::vector<ptr<Panel>>;

  class IManager {

public:
    // TODO(??) make private
    entt::registry registry;
    std::map<std::string, ptr<Element>> lookup;
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
      ptr<Element> entity,
      bool inside,
      bool interactive,
      bool mouseWentUp,
      bool mouseWentDown
    ) {
      bool result = false;

      if ( entity->id == _context.active ) {
        if ( mouseWentUp ) {
          if ( entity->id == _context.hot )
            result = true;// do the button action

          _context.active = "";
        }
      }
      else if ( entity->id == _context.hot ) {
        if ( mouseWentDown && interactive )
          _context.active = entity->id;
      }

      if ( inside ) {
        // if ( _context.active == entt::null && interactive ) {
        if ( _context.active == "" ) {
          _context.hot = entity->id;

          if ( mouseWentDown && interactive )
            _context.active = entity->id;
        }
      }

      return result;
    }

    // TODO(rf) this shouldnt use the selection system directly
    void DrawManagerDebugInfo() {
      DrawRectangle( GetScreenWidth() - 300, 102, 200, 24.0f, BLACK );
      std::string foo = "hot: " + ( _context.hot );
      DrawText( foo.c_str(), GetScreenWidth() - 300, 102, 24.0f, RED );

      DrawRectangle( GetScreenWidth() - 300, 152, 200, 24.0f, BLACK );
      std::string bar = "active: " + ( _context.active );
      DrawText( bar.c_str(), GetScreenWidth() - 300, 152, 24.0f, RED );

      DrawRectangle( GetScreenWidth() - 300, 202, 200, 24.0f, BLACK );
      std::string selected_ent =
        "entity: " + EntityIdToString( SelectionSystem::selected_entity );
      DrawText( selected_ent.c_str(), GetScreenWidth() - 300, 202, 24.0f, RED );
    }

    bool MouseIsOverUI() {
      // This is almost sufficent, but we need to account for panels too
      // not just items that can be active?
      return _context.active != "" || _context.hot != "";
    }

    void SetContextNull() {
      _context.hot = "";
      _context.active = "";
    }

    void ClearRegistry() {
      registry = {};
      registry.clear();
    }


private:
    Context _context = { "", "" };
    u32 _active_page_i = PageType::MainMenu;


    IManager() {}
    ~IManager() {}
  };

  inline IManager *Manager() {
    return IManager::Manager();
  }

  template<typename T>
  inline std::shared_ptr<T> Create( T element ) {
    auto created = std::make_shared<T>( element );
    created->SubscribeToMessages();
    Manager()->lookup.insert_or_assign( element.id, created );
    return created;
  }


};// namespace UI
