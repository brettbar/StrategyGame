#pragma once

#include "../shared/global.hpp"
#include "../shared/utils.hpp"
#include "../signals/events.hpp"
#include "../signals/updates.hpp"
#include "ui_shared.hpp"

namespace UI
{
  enum class Type : u32
  {
    INVALID,
    Panel,
    StackPanel,
    TextLabel,
    TextButton,
    TextureLabel,
    TextureButton,
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

  class Element
  {

    // Element() = delete;
    Element() = default;
    Element( std::string id ) : id( id ){};
    // ~Element() {
    //   // TODO Remove id from lookup
    // }

public:
    friend class PanelBuilder;
    friend class StackPanelBuilder;
    friend class TextLabelBuilder;
    friend class TextButtonBuilder;

    Type type = Type::INVALID;
    std::string id = "INVALID";
    bool enabled = false;
    bool starts_disabled = false;
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

    std::map<
      InterfaceUpdate::ID,
      std::function<void( Element &, InterfaceUpdate::Update )>>
      updates = {};

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

    void InitialEnable()
    {
      if ( starts_disabled )
        return;

      switch ( type )
      {
        case Type::INVALID:
          printf( "INVALID TYPE ENABLED\n" );
          assert( false );
          break;
        case Type::Panel:
        {
          enabled = true;
          Resize();
          Reposition();

          for ( Element &child: children )
          {
            child.InitialEnable();
            child.Resize();
            child.Reposition();
          }
        }
        break;
        case Type::StackPanel:
        {
          enabled = true;
          children[curr_index].InitialEnable();
          children[curr_index].Resize();
          children[curr_index].Reposition();
        }
        break;
        case Type::TextLabel:
        case Type::TextButton:
        {
          enabled = true;
        }
        break;
        default:
          enabled = true;
          break;
      }
    }

    void Enable()
    {
      switch ( type )
      {
        case Type::INVALID:
          printf( "INVALID TYPE ENABLED\n" );
          assert( false );
          break;
        case Type::Panel:
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
        case Type::StackPanel:
        {
          enabled = true;
          children[curr_index].Enable();
          children[curr_index].Resize();
          children[curr_index].Reposition();
        }
        break;
        case Type::TextLabel:
        case Type::TextButton:
        {
          enabled = true;
        }
        break;
        default:
          enabled = true;
          break;
      }
    }

    void Register()
    {
      switch ( type )
      {
        case Type::INVALID:
          printf( "INVALID TYPE ENABLED\n" );
          assert( false );
          break;
        case Type::Panel:
        {
          lookup.emplace( id, std::make_shared<Element>( *this ) );

          for ( Element &child: children )
          {
            child.Register();
          }
        }
        break;
        case Type::StackPanel:
        {
          lookup.emplace( id, std::make_shared<Element>( *this ) );
          children[curr_index].Register();
        }
        break;
        default:
          lookup.emplace( id, std::make_shared<Element>( *this ) );
          break;
      }
    }

    void Disable()
    {
      switch ( type )
      {
        case Type::Panel:
        {
          for ( Element child: children )
          {
            child.Disable();
          }

          enabled = false;
        }
        break;
        case Type::StackPanel:
        {
          children[curr_index].Disable();
          enabled = false;
        }
        break;
        case Type::TextLabel:
        case Type::TextButton:
        {
          enabled = false;
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
        case Type::StackPanel:
        case Type::Panel:
          switch ( anchor )
          {
            case Anchor::Centered:
            {
              vec2 updated_pos = {
                ( (f32) GetScreenWidth() / 2 ) - ( transform.width / 2 ),
                ( (f32) GetScreenHeight() / 2 ) - ( transform.height / 2 ),
              };

              transform.x = updated_pos.x;
              transform.y = updated_pos.y;
            }
            break;
            case Anchor::TopRight:
            {
              vec2 updated_pos = {
                ( (f32) GetScreenWidth() ) - ( transform.width ), 0 };

              transform.x = updated_pos.x;
              transform.y = updated_pos.y;
            }
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
        case Type::INVALID:
          printf( "INVALID TYPE IN RESIZE\n" );
          assert( false );
          break;
        case Type::Panel:
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
        case Type::StackPanel:
        {
          children[curr_index].transform.x = transform.x;
          children[curr_index].transform.y = transform.y;
          children[curr_index].Resize();
        }
        break;
        case Type::TextButton:
        case Type::TextLabel:
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
        case Type::TextureButton:
        case Type::TextureLabel:
        {
          transform.width = texture.width * UI::SCALE;
          transform.height = texture.height * UI::SCALE;
        }
        break;
      }
    }

    void UpdateText( std::string new_text )
    {
      text = new_text;
    }

    void UpdateBackground( Color new_color )
    {
      background = new_color;
    }

    void UpdateClickable( bool new_clickable )
    {
      clickable = new_clickable;
    }


    // TODO rename or remove
    void Update()
    {
      if ( !enabled )
        return;

      switch ( type )
      {
        case Type::Panel:
        {
          if ( update_children )
            update_children( children );
        }
        break;
        default:
          break;
      }
    }

    void Draw()
    {
      if ( !enabled )
        return;

      switch ( type )
      {
        case Type::INVALID:
          printf( "INVALID TYPE IN DRAW\n" );
          assert( false );
          break;
        case Type::Panel:
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
        case Type::StackPanel:
        {
          DrawRectangleV(
            { transform.x, transform.y },
            { transform.width, transform.height },
            background
          );

          children[curr_index].Draw();
        }
        break;
        case Type::TextLabel:
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
        case Type::TextButton:
        {
          // TODO - do we really want this to black out like that
          // Caused me to lose my sanity for a month
          // Bruh
          // if ( !clickable )
          // {
          //   DrawRectangleV(
          //     { transform.x, transform.y },
          //     { transform.width, transform.height },
          //     Fade( BLACK, 0.5 )
          //   );

          //   DrawTextEx(
          //     Global::font_cache[hstr{ "font_romulus" }]->font,
          //     text.c_str(),
          //     {
          //       transform.x,
          //       transform.y,
          //     },
          //     font_size,
          //     2.0,
          //     text_color
          //   );
          // }
          // else
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
        case Type::TextureLabel:
        case Type::TextureButton:
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

    // void ReceiveUpdate( const InterfaceUpdate::Data &update )
    // {
    //   for ( InterfaceUpdate::ID subscribed_update_id: subscribed_updates )
    //   {
    //     // TODO(??) we are crashing here sometimes not sure why
    //     if ( subscribed_update_id == update.update_id )
    //     {
    //       switch ( update.type )
    //       {
    //         case InterfaceUpdate::Type::Enabled:
    //           if ( update.on )
    //             Enable();
    //           else
    //             Disable();
    //           break;
    //         case InterfaceUpdate::Type::Text:
    //           if ( update.targeted )
    //           {
    //             if ( update.target == id )
    //             {
    //               printf(
    //                 "msg.target %s, id %s\n", update.target.c_str(), id.c_str()
    //               );

    //               text = update.updated_text;
    //             }
    //           }
    //           else
    //           {
    //             text = update.updated_text;
    //           }
    //           break;
    //         case InterfaceUpdate::Type::Background:
    //         {
    //           if ( update.targeted )
    //           {
    //             if ( update.target == id )
    //             {
    //               std::cout << "InterfaceUpdate::Type::BackgroundUpdate"
    //                         << " target: " << update.target << " us: " << id
    //                         << '\n';

    //               std::cout
    //                 << "We got: " << FormatRGB( update.updated_background )
    //                 << '\n';

    //               std::cout << "before: " << FormatRGB( background ) << '\n';

    //               background = update.updated_background;

    //               std::cout << "after: " << FormatRGB( background ) << '\n';
    //             }
    //           }
    //           else
    //           {
    //             background = update.updated_background;
    //           }
    //         }
    //         break;
    //         case InterfaceUpdate::Type::Clickable:
    //           if ( update.targeted )
    //           {
    //             if ( update.target == id )
    //             {
    //               clickable = update.clickable;
    //             }
    //           }
    //           else
    //           {
    //             clickable = update.clickable;
    //           }
    //           break;
    //         default:
    //           printf( "Error :: Panel deos not support message type\n" );
    //           break;
    //       }
    //       break;
    //     }
    //   }
    // }
    void ReceiveUpdate( const InterfaceUpdate::Update &update )
    {
      if ( updates.contains( update.id ) )
      {
        updates[update.id]( *this, update );
      }
    }

    void SubscribeToUpdates()
    {
      switch ( type )
      {
        case Type::Panel:
        {
          InterfaceUpdate::dispatcher.sink<InterfaceUpdate::Update>()
            .connect<&Element::ReceiveUpdate>( this );
          for ( auto &child: children )
          {
            child.SubscribeToUpdates();
          }
        }
        break;
        case Type::StackPanel:
        {
          InterfaceUpdate::dispatcher.sink<InterfaceUpdate::Update>()
            .connect<&Element::ReceiveUpdate>( this );
          children[curr_index].SubscribeToUpdates();
        }
        break;
        default:
          InterfaceUpdate::dispatcher.sink<InterfaceUpdate::Update>()
            .connect<&Element::ReceiveUpdate>( this );
          break;
      }
    }

    void UnsubscribeFromUpdates()
    {
      InterfaceUpdate::dispatcher.sink<InterfaceUpdate::Update>()
        .disconnect<&Element::ReceiveUpdate>( this );
    }
  };

  class AbstractBuilder
  {
protected:
    Element &_element;
    explicit AbstractBuilder( Element &element ) : _element( element ) {}

public:
    operator Element() const
    {
      return std::move( _element );
    };
  };

  class PanelBuilder : public AbstractBuilder
  {
    Element _element;

public:
    explicit PanelBuilder( std::string id ) : AbstractBuilder{ _element }
    {
      _element.type = Type::Panel;
      _element.id = id;
      _element.children_axis = Axis::Row;
      _element.children_horiz_align = Align::Start;
      _element.children_vert_align = Align::Start;
    }

    PanelBuilder &SetAnchor( Anchor anchor )
    {
      _element.anchor = anchor;
      return *this;
    }

    PanelBuilder &SetAxis( Axis axis )
    {
      _element.children_axis = axis;
      return *this;
    }


    PanelBuilder &Children( std::vector<Element> children )
    {
      _element.children = children;
      return *this;
    }

    PanelBuilder &UpdateChildren(
      std::function<void( std::vector<Element> & )> update_children
    )
    {
      _element.update_children = update_children;
      return *this;
    }

    PanelBuilder &Background( Color background )
    {
      _element.background = background;
      return *this;
    }

    PanelBuilder &Margins( Margins margins )
    {
      _element.margins = margins;
      return *this;
    }
  };

  class StackPanelBuilder : public AbstractBuilder
  {
    Element _element;

public:
    explicit StackPanelBuilder( std::string id ) : AbstractBuilder{ _element }
    {
      _element.type = Type::StackPanel;
      _element.id = id;
      _element.curr_index = 0;
    }

    StackPanelBuilder &Background( Color background )
    {
      _element.background = background;
      return *this;
    }

    StackPanelBuilder &Children( std::vector<Element> children )
    {
      _element.children = children;
      return *this;
    }
  };

  class TextLabelBuilder : public AbstractBuilder
  {
    Element _element;

public:
    explicit TextLabelBuilder( std::string id ) : AbstractBuilder{ _element }
    {
      _element.type = Type::TextLabel;
      _element.id = id;
    }

    TextLabelBuilder &On(
      InterfaceUpdate::ID update_id,
      std::function<void( Element &self, InterfaceUpdate::Update update )>
        update_fn
    )
    {
      _element.updates.emplace( update_id, update_fn );
      return *this;
    }

    TextLabelBuilder &Background( Color background )
    {
      _element.background = background;
      return *this;
    }

    TextLabelBuilder &SetText( std::string text, f32 font_size )
    {
      _element.text = text;
      _element.font_size = font_size;
      return *this;
    }

    TextLabelBuilder &SetText(
      std::string text,
      f32 font_size,
      Color text_color
    )
    {
      _element.text_color = text_color;
      return SetText( text, font_size );
    }
  };

  class TextButtonBuilder : public AbstractBuilder
  {
    Element _element;

public:
    explicit TextButtonBuilder( std::string id ) : AbstractBuilder{ _element }
    {
      _element.type = Type::TextButton;
      _element.id = id;
    }

    TextButtonBuilder &On(
      InterfaceUpdate::ID update_id,
      std::function<void( Element &self, InterfaceUpdate::Update update )>
        update_fn
    )
    {
      _element.updates.emplace( update_id, update_fn );
      return *this;
    }

    TextButtonBuilder &StartDisabled()
    {
      _element.starts_disabled = true;
      return *this;
    }

    TextButtonBuilder &Background( Color background )
    {
      _element.background = background;
      return *this;
    }

    TextButtonBuilder &SetText( std::string text, f32 font_size )
    {
      _element.text = text;
      _element.font_size = font_size;
      return *this;
    }

    TextButtonBuilder &SetText(
      std::string text,
      f32 font_size,
      Color text_color
    )
    {
      _element.text_color = text_color;
      return SetText( text, font_size );
    }

    TextButtonBuilder &SetEvent( InterfaceEvent::Data event )
    {
      _element.on_click = std::make_shared<InterfaceEvent::Data>( event );
      return *this;
    }


    TextButtonBuilder &Clickable( bool clickable )
    {
      _element.clickable = clickable;
      return *this;
    }
  };


  inline PanelBuilder Panel( std::string id )
  {
    return PanelBuilder{ id };
  }

  inline StackPanelBuilder StackPanel( std::string id )
  {
    return StackPanelBuilder{ id };
  }

  inline TextLabelBuilder TextLabel( std::string id )
  {
    return TextLabelBuilder{ id };
  }

  inline TextButtonBuilder TextButton( std::string id )
  {
    return TextButtonBuilder{ id };
  }

};// namespace UI
