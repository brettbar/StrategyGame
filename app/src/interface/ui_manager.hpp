#pragma once

#include "../shared/global.hpp"
#include "../world/systems/selection_system.hpp"

#include "builders.hpp"
#include "element.hpp"

namespace UI
{
  void ClearContext();
  void CheckOverElem( bool );
  bool CheckInteraction();
  inline Element CreateDebugInfo();

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


  class IManager
  {

public:
    // TODO(??) make private
    entt::registry registry;
    std::array<sptr<Element>, NUM_PAGES> pages;
    // Element debug;

    bool over_any_elem = false;

    sptr<Element> &ActivePage()
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
      sptr<Element> &entity,
      bool clickable,
      bool inside,
      bool mouseWentUp,
      bool mouseWentDown
    )
    {
      bool result = false;

      if ( entity->id == _context.active )
      {
        if ( mouseWentUp )
        {
          if ( entity->id == _context.hot )
            result = true;// do the button action

          _context.active = "";
        }
      }
      else if ( entity->id == _context.hot && clickable )
      {
        if ( mouseWentDown )
          _context.active = entity->id;
      }

      if ( inside )
      {
        if ( _context.active == "" )
        {
          _context.hot = entity->id;

          if ( mouseWentDown )
            _context.active = entity->id;
        }
      }

      return result;
    }

    // TODO(rf) this shouldnt use the selection system directly
    // void DrawManagerDebugInfo()
    // {
    //   DrawRectangle( GetScreenWidth() - 300, 102, 200, 24.0f, BLACK );
    //   std::string foo = "hot: " + ( _context.hot );
    //   DrawText( foo.c_str(), GetScreenWidth() - 300, 102, 24.0f, RED );

    //   DrawRectangle( GetScreenWidth() - 300, 152, 200, 24.0f, BLACK );
    //   std::string bar = "active: " + ( _context.active );
    //   DrawText( bar.c_str(), GetScreenWidth() - 300, 152, 24.0f, RED );

    //   DrawRectangle( GetScreenWidth() - 300, 202, 200, 24.0f, BLACK );
    //   std::string selected_ent =
    //     "entity: " + EntityIdToString( SelectionSystem::selected_entity );
    //   DrawText( selected_ent.c_str(), GetScreenWidth() - 300, 202, 24.0f, RED );
    // }

    bool MouseIsOverUI()
    {
      // This is almost sufficent, but we need to account for panels too
      // not just items that can be active?
      return _context.active != "" || _context.hot != "" ||
             Manager()->over_any_elem;
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

    std::string Hot()
    {
      return _context.hot;
    }
    std::string Active()
    {
      return _context.active;
    }


private:
    Context _context = { "", "" };
    u32 _active_page_i = PageType::MainMenu;


    // IManager() : debug( CreateDebugInfo() ) {}
    IManager() {}
    ~IManager() {}
  };


  inline IManager *Manager()
  {
    return IManager::Manager();
  }

  // inline Element CreateDebugInfo()
  // {
  //   // auto update_debug_info = [](
  //   //                            std::map<std::string, bool> &,
  //   //                            std::vector<Element> &children
  //   //                          ) {
  //   //   for ( auto &child: children )
  //   //   {

  //   //     if ( child.id == "fps" )
  //   //     {
  //   //       child.UpdateText( "fps: " + std::to_string( GetFPS() ) );
  //   //     }
  //   //     else if ( child.id == "hot" )
  //   //     {
  //   //       child.UpdateText( "hot: " + ( Manager()->Hot() ) );
  //   //     }
  //   //     else if ( child.id == "active" )
  //   //     {
  //   //       child.UpdateText( "active: " + ( Manager()->Active() ) );
  //   //     }
  //   //     else if ( child.id == "element.background" )
  //   //     {
  //   //       if ( lookup.contains( Manager()->Hot() ) )
  //   //       {
  //   //         // std::cout << "Updating color with "
  //   //         //           << FormatRGB( lookup.at( Manager()->Hot() )->Background()
  //   //         //              )
  //   //         //           << '\n';

  //   //         child.UpdateText(
  //   //           "element.background: " +
  //   //           FormatRGB( lookup.at( Manager()->Hot() )->background )
  //   //         );
  //   //       }
  //   //       else
  //   //       {
  //   //         child.UpdateText( "element.background: " );
  //   //       }
  //   //     }

  //   //     else if ( child.id == "selected" )
  //   //     {
  //   //       child.UpdateText(
  //   //         "entity: " +
  //   //         EntityIdToString( SelectionSystem::GetSelectedEntity() )
  //   //       );
  //   //     }
  //   //   }
  //   // };

  //   // return Panel( "debug_info" )
  //   //   .Axis( Axis::Column )
  //   //   .Anchor( Anchor::TopRight )
  //   //   .UpdateChildren( update_debug_info )
  //   //   .Background( BLUE )
  //   //   .Children( {
  //   //     TextLabel( "fps" ).Background( BLACK ).Text( "fps: ", 18.0f, GREEN ),
  //   //     TextLabel( "hot" ).Background( BLACK ).Text( "hot: ", 18.0f ),
  //   //     TextLabel( "active" ).Background( BLACK ).Text( "active: ", 18.0f ),
  //   //     TextLabel( "element.background" )
  //   //       .Background( BLACK )
  //   //       .Text( "element.background: ", 18.0f ),
  //   //     TextLabel( "selected" ).Text( "entity: ", 18.0f ).Background( BLACK ),
  //   //   } );
  //   return Element{};
  // }

  inline void Interact(
    sptr<Element> &element,
    bool mouse_went_up,
    bool mouse_went_down
  )
  {
    if ( !element->enabled )
      return;

    switch ( element->type )
    {
      case Type::GridPanel:
      {
        for ( GridPanelElement::Slot &slot: element->grid_panel->children )
        {
          Interact( slot.child, mouse_went_up, mouse_went_down );
        }
      }
      break;
      case Type::StackPanel:
      {
        // Interact(
        //   element->grid_panel->children[element->curr_index],
        //   mouse_went_up,
        //   mouse_went_down
        // );

        // if ( !Manager()->over_any_elem )
        // {
        //   Manager()->SetContextNull();
        // }
      }
      break;
      case Type::TextureButton:
      case Type::TextButton:
      {
        bool inside =
          CheckCollisionPointRec( GetMousePosition(), element->transform );

        if ( !Manager()->over_any_elem )
          Manager()->over_any_elem = inside;

        bool clickable = false;

        if ( element->type == Type::TextureButton )
        {
          // TODO
          // clickable = element->texture_button->clickable;
        }
        else if ( element->type == Type::TextButton )
        {
          clickable = element->text_button->clickable;
        }

        if ( Manager()->DoInteraction(
               element, clickable, inside, mouse_went_up, mouse_went_down
             ) )
        {
          if ( clickable )
          {
            std::cout << "Interactable element clicked" << '\n';

            element->FireEvent();
          }
        }
      }
      break;
      case Type::TextureLabel:
      case Type::TextLabel:
      {
        bool inside =
          CheckCollisionPointRec( GetMousePosition(), element->transform );

        if ( !Manager()->over_any_elem )
          Manager()->over_any_elem = inside;

        if ( Manager()->DoInteraction(
               element, false, inside, mouse_went_up, mouse_went_down
             ) )
        {
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
      case Type::GridPanel:
      {
        for ( GridPanelElement::Slot &slot: grid_panel->children )
        {
          slot.child->Destroy();
        }
        // Manager()->lookup.erase( id );
      }
      break;
      case Type::StackPanel:
      {
        // grid_panel->children[curr_index]->Destroy();
        // // Manager()->lookup.erase( id );
      }
      break;
      default:
        // Manager()->lookup.erase( id );
        break;
    }
  }

};// namespace UI
