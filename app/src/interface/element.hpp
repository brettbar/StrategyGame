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
    GridPanel,
    DataPanel,
    StackPanel,
    TextLabel,
    TextButton,
    TextureLabel,
    TextureButton,
  };

  struct Margins
  {
    u32 left;
    u32 right;
    u32 top;
    u32 bottom;
  };

  struct Element;


  struct GridPanelElement
  {
    struct Slot
    {
      struct Dimensions
      {
        u32 start_col;
        u32 end_col;
        u32 start_row;
        u32 end_row;
      };

      Dimensions dims;
      sptr<Element> child;
    };

    // Panel
    bool abs_size = false;
    u32 num_cols = 0;
    u32 num_rows = 0;
    list<Slot> children = {};

    u32 GridIndex( u32, u32 );

    void Initialize( rect );
    void Enable( rect );
    void PanelRegister();
    void PanelDisable();
    void PanelResize( rect );
    void PanelReposition( rect );
    void PanelDraw( rect );
    void PanelExecuteInterfaceUpdate( const InterfaceUpdate::Update & );

    GridPanelElement() = delete;
    GridPanelElement( u32 c, u32 r ) : num_cols( c ), num_rows( r ) {}
  };

  struct StackPanelElement
  {
    u32 curr_index = 0;
    list<sptr<Element>> tabs;

    StackPanelElement() = delete;
    StackPanelElement( list<sptr<Element>> tabs ) : tabs( tabs ) {}
  };

  struct TextLabelElement
  {
    str text = "INVALID";
    u32 font_size = 14;
    Color text_color = WHITE;

    // TODO(rf) probably can remove all together
    bool dynamic = false;

    TextLabelElement() = delete;
    TextLabelElement( str text, u32 font_size )
        : text( text ), font_size( font_size )
    {
    }

    void Draw( Color, rect );
  };

  struct TextButtonElement
  {
    bool clickable = true;
    sptr<TextLabelElement> label;
    sptr<InterfaceEvent::Data> on_click;

    TextButtonElement() = delete;
    TextButtonElement( sptr<TextLabelElement> label ) : label( label ) {}
    TextButtonElement(
      sptr<TextLabelElement> label,
      sptr<InterfaceEvent::Data> event
    )
        : label( label ), on_click( event )
    {
    }

    inline void Draw( Color background, rect transform )
    {
      label->Draw( background, transform );
    }
  };

  struct TextureLabelElement
  {
    Texture2D texture = Texture2D();
    TextureLabelElement() = delete;
    TextureLabelElement( Texture2D texture ) : texture( texture ) {}
  };

  struct TextureButtonElement
  {
    bool clickable = true;
    sptr<TextureLabelElement> label;
    sptr<InterfaceEvent::Data> on_click = nullptr;
    TextureButtonElement() = delete;
    TextureButtonElement(
      sptr<TextureLabelElement> label,
      sptr<InterfaceEvent::Data> on_click
    )
        : label( label ), on_click( on_click )
    {
    }
  };

  struct Element
  {
    Element() = delete;

    // GridPanel
    Element( str id, sptr<GridPanelElement> grid_panel )
        : type( Type::GridPanel ), id( id ), grid_panel( grid_panel )
    {
    }

    // StackPanel
    Element( str id, sptr<StackPanelElement> stack_panel )
        : type( Type::StackPanel ), id( id ), stack_panel( stack_panel )
    {
    }

    // TextLabel
    Element( str id, sptr<TextLabelElement> text_label )
        : type( Type::TextLabel ), id( id ), text_label( text_label )

    {
    }

    // TextButton
    Element( str id, sptr<TextButtonElement> text_button )
        : type( Type::TextButton ), id( id ), text_button( text_button )
    {
    }

    // TextureLabel
    Element( str id, sptr<TextureLabelElement> texture_label )
        : type( Type::TextureLabel ), id( id ), texture_label( texture_label )
    {
    }

    // TextureButton
    Element( str id, sptr<TextureButtonElement> texture_button )
        : type( Type::TextureButton ), id( id ),
          texture_button( texture_button )
    {
    }


    Type type;
    str id = "INVALID";
    bool enabled = false;
    bool starts_disabled = false;
    Color background = Fade( BLACK, 0.0 );
    rect transform = rect{ 0, 0, 0, 0 };
    Margins margins = Margins{ 0, 0, 0, 0 };

    sptr<GridPanelElement> grid_panel = nullptr;
    sptr<StackPanelElement> stack_panel = nullptr;
    sptr<TextLabelElement> text_label = nullptr;
    sptr<TextButtonElement> text_button = nullptr;
    sptr<TextureLabelElement> texture_label = nullptr;
    sptr<TextureButtonElement> texture_button = nullptr;

    //DataPanel
    std::map<std::string, Element> data_points = {};
    std::function<void( Element & )> self_update = {};

    list<InterfaceUpdate::ID> subscribed_updates = {};
    std::map<
      InterfaceUpdate::ID,
      std::function<void( Element &, InterfaceUpdate::Update )>>
      updates = {};


    // High Level
    void Initialize();
    void Enable();
    void Register();
    void Disable();
    void Destroy();
    // TODO should we take in to account ui scale
    void ResizeRecursive();
    void RepositionRecursive();
    void Draw();
    void UpdateText( std::string );
    void UpdateBackground( Color );
    void UpdateClickable( bool new_clickable );
    void ExecuteInterfaceUpdate( const InterfaceUpdate::Update & );
    void Update();
    void FireEvent();
  };

};// namespace UI
