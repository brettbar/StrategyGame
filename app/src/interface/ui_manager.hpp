#pragma once

#include "../shared/global.hpp"
#include "../world/systems/selection_system.hpp"

#include "element.hpp"

namespace UI
{
  void ClearContext();
  void CheckOverElem( bool );
  bool CheckInteraction();

  enum PageType : u32
  {
    MainMenu,
    FactionSelectMenu,
    SinglePlayerLobby,
    ModalMenu,
    Campaign,
    LobbyBrowser,
    Lobby,
    NUM_PAGES,
  };

  // Panel?
  using Page = std::vector<Element>;

  class IManager
  {

public:
    // TODO(??) make private
    entt::registry registry;
    // std::map<std::string, ptr<Element>> lookup;
    std::array<Page, NUM_PAGES> pages;

    bool over_any_elem = false;

    Page &ActivePage()
    {
      return pages[_active_page_i];
    }

    void SetScene( PageType page )
    {
      _active_page_i = page;
    }

    static IManager *Manager()
    {
      static IManager instance;
      return &instance;
    }

    IManager( IManager const & ) = delete;
    void operator=( const IManager & ) = delete;

    bool DoInteraction(
      Element entity,
      bool inside,
      bool mouseWentUp,
      bool mouseWentDown
    )
    {
      bool result = false;

      if ( entity.ID() == _context.active )
      {
        if ( mouseWentUp )
        {
          if ( entity.ID() == _context.hot )
            result = true;// do the button action

          _context.active = "";
        }
      }
      else if ( entity.ID() == _context.hot )
      {
        if ( mouseWentDown )
          _context.active = entity.ID();
      }

      if ( inside )
      {
        if ( _context.active == "" )
        {
          _context.hot = entity.ID();

          if ( mouseWentDown )
            _context.active = entity.ID();
        }
      }

      return result;
    }

    // TODO(rf) this shouldnt use the selection system directly
    void DrawManagerDebugInfo()
    {
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

    bool MouseIsOverUI()
    {
      // This is almost sufficent, but we need to account for panels too
      // not just items that can be active?
      return _context.active != "" || _context.hot != "";
    }

    void SetContextNull()
    {
      _context.hot = "";
      _context.active = "";
    }

    void ClearRegistry()
    {
      registry = {};
      registry.clear();
    }


private:
    Context _context = { "", "" };
    u32 _active_page_i = PageType::MainMenu;


    IManager() {}
    ~IManager() {}
  };

  inline IManager *Manager()
  {
    return IManager::Manager();
  }

  inline void Element::Interact( bool mouse_went_up, bool mouse_went_down )
  {
    if ( !this || !enabled )
      return;

    switch ( type )
    {
      case Type::Panel:
      {
        for ( Element &child: children )
        {
          if ( child.enabled )
          {
            child.Interact( mouse_went_up, mouse_went_down );
          }
        }
      }
      case Type::StackPanel:
      {
        children[curr_index].Interact( mouse_went_up, mouse_went_down );

        if ( !Manager()->over_any_elem )
        {
          Manager()->SetContextNull();
        }
      }
      break;
      case Type::TextButton:
      {
        bool inside = CheckCollisionPointRec( GetMousePosition(), transform );

        if ( !Manager()->over_any_elem )
          Manager()->over_any_elem = inside;

        if ( Manager()->DoInteraction(
               *this, inside, mouse_went_up, mouse_went_down
             ) )
        {
          if ( clickable )
          {
            std::cout << "INTERACTION DETECTED!!!" << std::endl;

            FireEvent();
          }
        }
      }
      break;
      default:
        break;
    }
  }

  inline void Element::Destroy()
  {
    switch ( type )
    {
      case Type::Panel:
      {
        for ( Element &child: children )
        {
          child.Destroy();
        }
        // Manager()->lookup.erase( id );
      }
      break;
      case Type::StackPanel:
      {
        children[curr_index].Destroy();
        // Manager()->lookup.erase( id );
      }
      break;
      default:
        // Manager()->lookup.erase( id );
        break;
    }
  }

};// namespace UI
