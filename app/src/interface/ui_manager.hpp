#pragma once

#include "../shared/global.hpp"
#include "../shared/signals.hpp"
#include "../world/systems/selection_system.hpp"
#include "ui_shared.hpp"

namespace UI
{

  enum Type : u32
  {
    INVALID,
    PANEL,
    STACK_PANEL,
    TEXT_LABEL,
    TEXT_BUTTON,
    TEXTURE_LABEL,
    TEXTURE_BUTTON,
  };

  enum class Anchor
  {
    Free,
    TopLeft,
    TopMid,
    TopRight,
    MidLeft,
    Centered,
    MidRight,
    BottomLeft,
    BottomMid,
    BottomRight,
  };

  enum class Axis
  {
    INVALID,
    Row,
    Column,
  };

  enum class Align
  {
    INVALID,
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
    Type type = Type::INVALID;
    bool enabled = false;
    std::string id = "INVALID";
    Color background = BLACK;
    rect transform = rect{ 0, 0, 0, 0 };
    Margins margins = Margins{ 0, 0, 0, 0 };

    // Panel
    Anchor anchor = Anchor::Free;// Might apply to others as well?
    Axis children_axis = Axis::INVALID;
    Align children_horiz_align = Align::INVALID;
    Align children_vert_align = Align::INVALID;
    bool abs_size = false;
    std::vector<Element> children = {};
    std::function<void( std::vector<Element> & )> update_children = {};
    std::vector<InterfaceUpdate::ID> subscribed_updates = {};

    // StackPanel
    u32 curr_index = 0;

    // TextLabel
    std::string text = "INVALID";
    i32 font_size = 14;
    Color text_color = WHITE;
    // TODO(rf) probably can remove all together
    bool dynamic = false;

    // TextButton
    bool clickable = true;
    ptr<InterfaceEvent::Data> on_click = nullptr;

    // TextureLabel
    Texture2D texture = Texture2D();


    void Enable()
    {
      switch ( type )
      {
        case INVALID:
          printf( "INVALID TYPE ENABLED\n" );
          assert( false );
          break;
        case PANEL:
        {
          enabled = true;
          Resize();
          Reposition();

          for ( Element &child: children )
          {
            child.Enable();
            child.Resize();
            child.Reposition();
          }
        }
        break;
        case STACK_PANEL:
        {
          enabled = true;
          children[curr_index].Enable();
          children[curr_index].Resize();
          children[curr_index].Reposition();
        }
        break;
        case TEXT_LABEL:
        case TEXT_BUTTON:
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
        case PANEL:
        {
          for ( Element child: children )
          {
            child.Disable();
          }

          enabled = false;
        }
        break;
        case STACK_PANEL:
        {
          children[curr_index].Disable();
          enabled = false;
        }
        break;
        case TEXT_LABEL:
        case TEXT_BUTTON:
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

    void Destroy();


    void Reposition()
    {
      switch ( type )
      {
        case STACK_PANEL:
        case PANEL:
          switch ( anchor )
          {
            case Anchor::Centered:
            {
              vec2 updated_pos = {
                ( (f32) GetScreenWidth() / 2 ) -
                  ( transform.width * SCALE / 2.0f ),
                ( (f32) GetScreenHeight() / 2 ) - transform.height * SCALE,
              };

              transform.x = updated_pos.x;
              transform.y = updated_pos.y;
            }
            break;
            default:
              break;
          }

          for ( Element &child: children )
          {
            child.Reposition();
          }
          break;
        default:
          break;
      }
    }

    void Resize()
    {
      if ( !enabled )
        return;

      switch ( type )
      {
        case INVALID:
          printf( "INVALID TYPE IN RESIZE\n" );
          assert( false );
          break;
        case PANEL:
        {
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
        case STACK_PANEL:
        {
          children[curr_index].transform.x = transform.x;
          children[curr_index].transform.y = transform.y;
          children[curr_index].Resize();
        }
        break;
        case TEXT_BUTTON:
        case TEXT_LABEL:
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
        case TEXTURE_BUTTON:
        case TEXTURE_LABEL:
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
        case PANEL:
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
        case STACK_PANEL:
        {
          DrawRectangleV(
            { transform.x, transform.y },
            { transform.width, transform.height },
            background
          );

          children[curr_index].Draw();
        }
        break;
        case TEXT_LABEL:
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
        case TEXT_BUTTON:
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
        case TEXTURE_LABEL:
        case TEXTURE_BUTTON:
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
      if ( on_click )
        InterfaceEvent::event_emitter.publish( *on_click );
    }

    void Interact( bool mouse_went_up, bool mouse_went_down );


    void ReceiveMessage( const InterfaceUpdate::Data &msg )
    {
      // printf( "%s\n", this->id.c_str() );

      for ( InterfaceUpdate::ID msg_id: subscribed_updates )
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
    std::map<std::string, ptr<Element>> lookup;
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

  inline void Element::Interact( bool mouse_went_up, bool mouse_went_down )
  {
    if ( !enabled )
      return;

    switch ( type )
    {
      case PANEL:
      {
        for ( Element &child: children )
        {
          if ( child.enabled )
          {
            child.Interact( mouse_went_up, mouse_went_down );
          }
        }
      }
      case STACK_PANEL:
      {
        children[curr_index].Interact( mouse_went_up, mouse_went_down );

        if ( !Manager()->over_any_elem )
        {
          Manager()->SetContextNull();
        }
      }
      break;
      case TEXT_BUTTON:
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
      case PANEL:
      {
        for ( Element &child: children )
        {
          child.Destroy();
        }
        Manager()->lookup.erase( id );
      }
      break;
      case STACK_PANEL:
      {
        children[curr_index].Destroy();
        Manager()->lookup.erase( id );
      }
      break;
      default:
        Manager()->lookup.erase( id );
        break;
    }
  }

};// namespace UI
