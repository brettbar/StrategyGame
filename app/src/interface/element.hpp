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
    DataPanel,
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
    friend class DataPanelBuilder;
    friend class StackPanelBuilder;
    friend class TextLabelBuilder;
    friend class TextButtonBuilder;
    friend class TextureLabelBuilder;
    friend class TextureButtonBuilder;

    Type type = Type::INVALID;
    std::string id = "INVALID";
    bool enabled = false;
    bool starts_disabled = false;
    bool fixed_size = false;
    Color background = BLACK;
    rect transform = rect{ 0, 0, 0, 0 };
    Margins margins = Margins{ 0, 0, 0, 0 };

    // Panel
    Anchor anchor = Anchor::Free;// Might apply to others as well?
    Axis children_axis = Axis::INVALID;
    Align children_horiz_align = Align::INVALID;
    Align children_vert_align = Align::INVALID;

    std::vector<Element> children = {};

    //DataPanel
    std::map<std::string, Element> data_points = {};
    std::function<void( std::map<std::string, Element> & )> update_data = {};


    std::function<void( std::map<std::string, bool> &, std::vector<Element> & )>
      update_children = {};


    std::vector<InterfaceUpdate::ID> subscribed_updates = {};

    std::map<
      InterfaceUpdate::ID,
      std::function<void( Element &, InterfaceUpdate::Update )>>
      updates = {};

    // StackPanel
    // TODO this should probably be made private
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
        case Type::DataPanel:
        {
          enabled = true;
          Resize();
          Reposition();

          for ( auto &pair: data_points )
          {
            Element &child = pair.second;

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
        case Type::TextureLabel:
        case Type::TextureButton:
        {
          enabled = true;
        }
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
        case Type::DataPanel:
        {
          enabled = true;
          Resize();
          Reposition();

          for ( auto &pair: data_points )
          {
            Element &child = pair.second;
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
        case Type::TextureLabel:
        case Type::TextureButton:
        {
          enabled = true;
        }
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
        case Type::DataPanel:
        {
          lookup.emplace( id, std::make_shared<Element>( *this ) );

          for ( auto &pair: data_points )
          {
            Element &child = pair.second;
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
        case Type::INVALID:
        {
          std::cout << "ERROR :: "
                    << "Invalid element type found in Disable()" << '\n';
        }
        break;
        case Type::Panel:
        {
          for ( Element &child: children )
          {
            child.Disable();
          }

          enabled = false;
        }
        break;
        case Type::DataPanel:
        {
          for ( auto &pair: data_points )
          {
            Element &child = pair.second;
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
        case Type::TextureLabel:
        case Type::TextureButton:
        {
          enabled = false;
        }
        break;
      }
    }

    void Destroy();

    void SwitchChild( u32 index )
    {
      if ( index > children.size() )
      {
        std::cout << "ERROR :: "
                  << "StackPanel tried to switch to index greater than its "
                     "number of children."
                  << "\n";
        return;
      }
      assert( type == Type::StackPanel );
      children[curr_index].Disable();
      curr_index = index;
      children[curr_index].Enable();
    }


    void Reposition()
    {

      bool is_panel = type == Type::Panel || type == Type::DataPanel ||
                      type == Type::StackPanel;

      if ( !is_panel )
        return;

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
        break;
        case Anchor::BottomMid:
        {
          vec2 updated_pos = {
            ( (f32) GetScreenWidth() / 2 ) - ( transform.width / 2 ),
            ( (f32) GetScreenHeight() ) - ( transform.height ),
          };

          transform.x = updated_pos.x;
          transform.y = updated_pos.y;
        }
        break;
        default:
          break;
      }

      switch ( type )
      {
        case Type::Panel:
        {
          for ( Element &child: children )
          {
            child.Reposition();
          }
        }
        break;
        case Type::DataPanel:
        {
          for ( auto &pair: data_points )
          {
            Element &child = pair.second;
            child.Reposition();
          }
        }
        break;
        case Type::StackPanel:
        {
          children[curr_index].Reposition();
        }
        break;
        default:
          break;
      }
    }

    // TODO why are we Repositioning panel children in this function?
    // Shouldnt it be in Reposition fn?
    // TODO should we take in to account ui scale
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

          UpdateChildren();

          for ( Element &child: children )
          {
            // TODO not sure if this is right
            if ( !child.enabled )
              continue;

            child.Resize();
          }

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

          if ( !fixed_size )
          {
            if ( children_axis == Axis::Row )
            {
              transform.width = total_width;
              transform.height = tallest_child;
            }
            else
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
        case Type::DataPanel:
        {
          f32 total_height = 0;
          f32 total_width = 0;
          f32 tallest_child = 0;
          f32 widest_child = 0;
          f32 end_of_last_x = transform.x;
          f32 end_of_last_y = transform.y;

          UpdateChildren();

          for ( auto &pair: data_points )
          {
            Element &child = pair.second;
            // TODO not sure if this is right
            if ( !child.enabled )
              continue;

            child.Resize();
          }

          for ( auto &pair: data_points )
          {
            Element &child = pair.second;
            if ( !child.enabled )
              continue;

            total_width += child.transform.width;
            total_height += child.transform.height;

            if ( child.transform.width > widest_child )
              widest_child = child.transform.width;

            if ( child.transform.height > tallest_child )
              tallest_child = child.transform.height;
          }

          if ( !fixed_size )
          {
            if ( children_axis == Axis::Row )
            {
              transform.width = total_width;
              transform.height = tallest_child;
            }
            else
            {
              transform.width = widest_child;
              transform.height = total_height;
            }
          }


          for ( auto &pair: data_points )
          {
            Element &child = pair.second;

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
          transform.width = children[curr_index].transform.width;
          transform.height = children[curr_index].transform.height;
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

    void ExecuteInterfaceUpdate( const InterfaceUpdate::Update &update )
    {
      switch ( type )
      {
        case Type::Panel:
        {
          if ( updates.contains( update.id ) )
            updates[update.id]( *this, update );

          for ( auto &child: children )
          {
            child.ExecuteInterfaceUpdate( update );
          }
        }
        break;
        case Type::DataPanel:
        {
          if ( updates.contains( update.id ) )
            updates[update.id]( *this, update );

          for ( auto &pair: data_points )
          {
            Element &child = pair.second;
            child.ExecuteInterfaceUpdate( update );
          }
        }
        break;
        case Type::StackPanel:
        {
          if ( updates.contains( update.id ) )
            updates[update.id]( *this, update );

          children[curr_index].ExecuteInterfaceUpdate( update );
        }
        break;
        default:
          if ( updates.contains( update.id ) )
            updates[update.id]( *this, update );
          break;
      }
    }

    void UpdateChildren()
    {
      if ( !enabled )
        return;

      switch ( type )
      {
        case Type::Panel:
        {
          // TODO rf this should be removed now that we have data panels
          if ( update_children )
          {
            std::map<std::string, bool> existing_ids = {};

            for ( auto &child: children )
            {
              existing_ids.emplace( child.id, true );
            }

            update_children( existing_ids, children );
          }
        }
        break;
        case Type::DataPanel:
        {
          if ( update_data )
          {
            update_data( data_points );
          }
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
        case Type::DataPanel:
        {
          DrawRectangleV(
            { transform.x, transform.y },
            { transform.width, transform.height },
            background
          );

          for ( auto &pair: data_points )
          {
            Element &child = pair.second;
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

    PanelBuilder &On(
      InterfaceUpdate::ID update_id,
      std::function<void( Element &self, InterfaceUpdate::Update update )>
        update_fn
    )
    {
      _element.updates.emplace( update_id, update_fn );
      return *this;
    }

    PanelBuilder &StartDisabled()
    {
      _element.starts_disabled = true;
      return *this;
    }

    PanelBuilder &FixedSize( u32 width, u32 height )
    {
      _element.fixed_size = true;
      _element.transform.width = width;
      _element.transform.height = height;
      return *this;
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
      std::function<
        void( std::map<std::string, bool> &, std::vector<Element> & )>
        update_children
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

  class DataPanelBuilder : public AbstractBuilder
  {
    Element _element;

public:
    explicit DataPanelBuilder( std::string id ) : AbstractBuilder{ _element }
    {
      _element.type = Type::DataPanel;
      _element.id = id;
      _element.curr_index = 0;
      _element.children_axis = Axis::Row;
      _element.children_horiz_align = Align::Start;
      _element.children_vert_align = Align::Start;
    }

    DataPanelBuilder &DataPoints( std::map<std::string, Element> data_points )
    {
      _element.data_points = data_points;
      return *this;
    }

    DataPanelBuilder &UpdateData(
      std::function<void( std::map<std::string, Element> & )> update_data
    )
    {
      _element.update_data = update_data;
      return *this;
    }

    DataPanelBuilder &SetAxis( Axis axis )
    {
      _element.children_axis = axis;
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

    StackPanelBuilder &On(
      InterfaceUpdate::ID update_id,
      std::function<void( Element &self, InterfaceUpdate::Update update )>
        update_fn
    )
    {
      _element.updates.emplace( update_id, update_fn );
      return *this;
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

    TextLabelBuilder &Margins( Margins margins )
    {
      _element.margins = margins;
      return *this;
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

  class TextureLabelBuilder : public AbstractBuilder
  {
    Element _element;

public:
    explicit TextureLabelBuilder( std::string id ) : AbstractBuilder{ _element }
    {
      _element.type = Type::TextureButton;
      _element.id = id;
    }

    TextureLabelBuilder &SetTexture( std::string texture_id )
    {
      _element.texture =
        Global::texture_cache[hstr{ texture_id.c_str() }]->texture;
      return *this;
    }
  };

  class TextureButtonBuilder : public AbstractBuilder
  {
    Element _element;

public:
    explicit TextureButtonBuilder( std::string id )
        : AbstractBuilder{ _element }
    {
      _element.type = Type::TextureButton;
      _element.id = id;
      _element.texture = Global::texture_cache[hstr{ id.c_str() }]->texture;
    }

    TextureButtonBuilder &SetEvent( InterfaceEvent::Data event )
    {
      _element.on_click = std::make_shared<InterfaceEvent::Data>( event );
      return *this;
    }
  };


  inline PanelBuilder Panel( std::string id )
  {
    return PanelBuilder{ id };
  }

  inline DataPanelBuilder DataPanel( std::string id )
  {
    return DataPanelBuilder{ id };
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

  inline TextureLabelBuilder TextureLabel( std::string id )
  {
    return TextureLabelBuilder{ id };
  }

  inline TextureButtonBuilder TextureButton( std::string id )
  {
    return TextureButtonBuilder{ id };
  }

};// namespace UI
