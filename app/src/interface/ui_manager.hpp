#pragma once

#include "../shared/global.hpp"
#include "../shared/signals.hpp"
#include "../world/systems/selection_system.hpp"
#include "ui_shared.hpp"

namespace UI
{

  enum Type : u32
  {
    Panel,
    StackPanel,
    TextLabel,
    TextButton,
    TextureLabel,
    TextureButton,
  };

  enum class Axis
  {
    Row,
    Column,
  };

  enum class Align
  {
    Start,
  };

  struct Margins
  {
    u32 left;
    u32 right;
    u32 top;
    u32 bottom;
  };

  void ClearContext();
  void CheckOverElem( bool );
  bool CheckInteraction();

  struct Element
  {
    Type type;
    bool enabled;
    std::string id;
    Color background;
    rect transform;
    Margins margins;

    // Panel
    Axis children_axis;
    Align children_horiz_align;
    Align children_vert_align;
    bool abs_size = false;
    // std::function<void( Panel & )> update;
    std::vector<Element> children;
    std::vector<InterfaceUpdate::ID> subscribed_messages;

    // StackPanel
    u32 curr_index = 0;

    // TextLabel
    std::string text;
    i32 font_size;
    Color text_color;
    // TODO(rf) probably can remove all together
    bool dynamic = false;

    // TextButton
    bool clickable = true;
    ptr<InterfaceEvent::Data> on_click = nullptr;

    // TextureLabel
    Texture2D texture;


    Element(
      std::string id,
      Color background,
      bool enabled,
      rect transform,
      Margins margins
    )
        : enabled( enabled ), id( id ), background( background ),
          transform( transform ), margins( margins )
    {
    }

    void Enable()
    {
      switch ( type )
      {
        case Panel:
        {
          enabled = true;
          Resize();

          for ( Element &child: children )
          {
            child.Enable();
            child.Resize();
          }
        }
        break;
        case StackPanel:
        {
          enabled = true;
          children[curr_index].Enable();
        }
        break;
        case TextLabel:
        {
          enabled = true;
          SubscribeToMessages();
        }
        break;
        default:
          enabled = true;
          break;
      }
    }

    void Disable()
    {
      switch ( type )
      {
        case Panel:
        {
          for ( Element child: children )
          {
            child.Disable();
          }

          enabled = false;
        }
        break;
        case StackPanel:
        {
          children[curr_index].Disable();
          enabled = false;
        }
        break;
        case TextLabel:
        {
          enabled = false;
          UnsubscribeFromMessages();
        }
        break;
        default:
          enabled = false;
          break;
      }
    }

    void Resize()
    {
      switch ( type )
      {
        case Panel:
        {
          if ( enabled )
            return;

          f32 total_height = 0;
          f32 total_width = 0;
          f32 tallest_child = 0;
          f32 widest_child = 0;
          f32 end_of_last_x = transform.x;
          f32 end_of_last_y = transform.y;

          Update();

          for ( Element &child: children )
          {
            // TODO not sure if this is right
            // if ( !child )
            //   continue;
            if ( !child.enabled )
              continue;

            child.Resize();
          }

          if ( !abs_size )
          {
            for ( Element &child: children )
            {
              if ( !child.enabled )
                continue;

              total_width += child.transform.width;
              total_height += child.transform.height;

              if ( child.transform.width > widest_child )
                widest_child = child.transform.width;

              if ( child.transform.height > tallest_child )
                tallest_child = child.transform.height;
            }

            if ( children_axis == Axis::Row )
            {
              transform.width = total_width;
              transform.height = tallest_child;
            }
            else if ( children_axis == Axis::Column )
            {
              transform.width = widest_child;
              transform.height = total_height;
            }
          }


          for ( Element &child: children )
          {
            if ( !child.enabled )
              continue;

            if ( children_axis == Axis::Row )
            {
              // 2. Set the child x position based on alignment style.
              switch ( children_horiz_align )
              {
                case Align::Start:
                {
                  child.transform.x = end_of_last_x + child.margins.left;
                  end_of_last_x = child.transform.x + child.transform.width +
                                  child.margins.right;
                }
                break;
              }

              // 3. Set the child y position based on alignment style.
              switch ( children_vert_align )
              {
                case Align::Start:
                {
                  child.transform.y = transform.y;
                }
                break;
              }
            }
            else if ( children_axis == Axis::Column )
            {
              // 2. Set the child x position based on alignment style.
              switch ( children_horiz_align )
              {
                case Align::Start:
                {
                  child.transform.x = transform.x;
                }
                break;
              }

              // 3. Set the child y position based on alignment style.
              switch ( children_vert_align )
              {
                case Align::Start:
                {
                  child.transform.y = end_of_last_y;
                  // + margins.top;
                  end_of_last_y = child.transform.y + child.transform.height;
                  // + margins.bottom;
                }
                break;
              }
            }
          }
        }
        break;
        case StackPanel:
        {
          children[curr_index].transform.x = transform.x;
          children[curr_index].transform.y = transform.y;
          children[curr_index].Resize();
        }
        break;
        case TextLabel:
        {
          const vec2 text_dims = MeasureTextEx(
            Global::font_cache[hstr{ "font_romulus" }]->font,
            text.c_str(),
            font_size,
            2.0f
          );

          transform.width = text_dims.x;
          transform.height = text_dims.y;
        }
        break;
        case TextureLabel:
        {
          transform.width = texture.width * UI::SCALE;
          transform.height = texture.height * UI::SCALE;
        }
        break;
      }
    }

    void Update()
    {
      // if ( update )
      //   update( *this );
    }

    void Draw()
    {
      if ( !enabled )
        return;

      switch ( type )
      {
        case Panel:
        {
          DrawRectangleV(
            { transform.x, transform.y },
            { transform.width, transform.height },
            background
          );

          for ( Element &child: children )
          {
            child.Draw();
          }
        }
        break;
        case StackPanel:
        {
          DrawRectangleV(
            { transform.x, transform.y },
            { transform.width, transform.height },
            background
          );

          children[curr_index].Draw();
        }
        break;
        case TextLabel:
        {
          DrawRectangleV(
            { transform.x, transform.y },
            { transform.width, transform.height },
            background
          );

          DrawTextEx(
            Global::font_cache[hstr{ "font_romulus" }]->font,
            text.c_str(),
            {
              transform.x,
              transform.y,
            },
            font_size,
            2.0,
            text_color
          );
        }
        break;
        case TextButton:
        {
          if ( !clickable )
          {
            DrawRectangleV(
              { transform.x, transform.y },
              { transform.width, transform.height },
              Fade( BLACK, 0.5 )
            );

            DrawTextEx(
              Global::font_cache[hstr{ "font_romulus" }]->font,
              text.c_str(),
              {
                transform.x,
                transform.y,
              },
              font_size,
              2.0,
              text_color
            );
          }
          else
          {
            DrawRectangleV(
              { transform.x, transform.y },
              { transform.width, transform.height },
              background
            );

            DrawTextEx(
              Global::font_cache[hstr{ "font_romulus" }]->font,
              text.c_str(),
              {
                transform.x,
                transform.y,
              },
              font_size,
              2.0,
              text_color
            );
          }
        }
        break;
        case TextureLabel:
        case TextureButton:
        {
          DrawTextureEx(
            texture, { transform.x, transform.y }, 0.0, SCALE, WHITE
          );
        }
        break;
      }
    }


    void FireEvent()
    {
      InterfaceEvent::event_emitter.publish( on_click );
      // switch ( on_click->type ) {
      //   case InterfaceEvent::Type::Basic: {
      //     InterfaceEvent::event_emitter.publish( *on_click );
      //   } break;
      //   case InterfaceEvent::Type::ButtonClick: {
      //     std::shared_ptr<InterfaceEvent::ButtonClick> button_click =
      //       std::dynamic_pointer_cast<InterfaceEvent::ButtonClick>( on_click );

      //     if ( button_click ) {
      //       printf(
      //         "Sending button click! %s %s\n",
      //         button_click->origin_id.c_str(),
      //         button_click->msg.c_str()
      //       );
      //       InterfaceEvent::event_emitter.publish( *button_click );
      //     }
      //   } break;
      //   case InterfaceEvent::Type::JoinLobby: {
      //     std::shared_ptr<InterfaceEvent::JoinLobby> join_lobby =
      //       std::dynamic_pointer_cast<InterfaceEvent::JoinLobby>( on_click );

      //     if ( join_lobby ) {
      //       InterfaceEvent::event_emitter.publish( *join_lobby );
      //     }
      //   } break;
      // }
    }

    void Interact( bool mouse_went_up, bool mouse_went_down );


    void ReceiveMessage( const InterfaceUpdate::Data &msg )
    {
      printf( "%s\n", this->id.c_str() );

      for ( InterfaceUpdate::ID msg_id: subscribed_messages )
      {
        if ( msg_id == msg.update_id )
        {
          switch ( msg.type )
          {
            case InterfaceUpdate::EnabledUpdate:
              if ( msg.on )
                Enable();
              else
                Disable();
              break;
            case InterfaceUpdate::TextUpdate:
              text = msg.updated_text;
              break;
            case InterfaceUpdate::BackgroundUpdate:
              background = msg.updated_background;
              break;
            case InterfaceUpdate::TargetedTextUpdate:
              printf( "TextLabel::ReceiveMessage msg type: %d\n", msg.type );

              printf(
                "msg.target %s, id %s\n", msg.target.c_str(), id.c_str()
              );

              if ( msg.target == id )
              {
                text = msg.updated_text;
              }
              break;
            case InterfaceUpdate::TargetedBackgroundUpdate:
              if ( msg.target == id )
              {
                background = msg.updated_background;
              }
              break;
            default:
              printf( "Error :: Panel deos not support message type\n" );
              break;
          }
          break;
        }
      }
    }


    void SubscribeToMessages()
    {
      InterfaceUpdate::dispatcher.sink<InterfaceUpdate::Data>()
        .connect<&Element::ReceiveMessage>( this );
    }

    void UnsubscribeFromMessages()
    {
      InterfaceUpdate::dispatcher.sink<InterfaceUpdate::Data>()
        .disconnect<&Element::ReceiveMessage>( this );
    }
  };


  enum PageType
  {
    MainMenu,
    FactionSelectMenu,
    SinglePlayerLobby,
    ModalMenu,
    Campaign,
    LobbyBrowser,
    Lobby,
  };

  // Panel?
  using Page = std::vector<Element>;

  class IManager
  {

public:
    // TODO(??) make private
    entt::registry registry;
    std::map<std::string, Element &> lookup;
    std::vector<Page> pages;

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

      if ( entity.id == _context.active )
      {
        if ( mouseWentUp )
        {
          if ( entity.id == _context.hot )
            result = true;// do the button action

          _context.active = "";
        }
      }
      else if ( entity.id == _context.hot )
      {
        if ( mouseWentDown )
          _context.active = entity.id;
      }

      if ( inside )
      {
        if ( _context.active == "" )
        {
          _context.hot = entity.id;

          if ( mouseWentDown )
            _context.active = entity.id;
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

  inline Element CreatePanel() {}

  // template<typename T>
  // inline Create( T element )
  // {
  //   auto created = std::make_shared<T>( element );
  //   created->SubscribeToMessages();
  //   Manager()->lookup.insert_or_assign( element.id, created );
  //   return created;
  // }

  inline void Element::Interact( bool mouse_went_up, bool mouse_went_down )
  {
    switch ( type )
    {
      case Panel:
      {
        for ( Element child: children )
        {
          if ( child.enabled )
          {
            child.Interact( mouse_went_up, mouse_went_down );
          }
        }
      }
      case StackPanel:
      {
        children[curr_index].Interact( mouse_went_up, mouse_went_down );

        if ( !Manager()->over_any_elem )
        {
          Manager()->SetContextNull();
        }
      }
      break;
      case TextButton:
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
    }
  }

};// namespace UI
