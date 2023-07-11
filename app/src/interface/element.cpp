#include "element.hpp"

namespace UI
{
  void Element::Enable()
  {
    switch ( type )
    {
      case Type::Panel:
        PanelEnable();
        break;
      case Type::DataPanel:
        DataPanelEnable();
        break;
      case Type::StackPanel:
        StackPanelEnable();
        break;
      default:
        enabled = true;
        break;
    }
  }

  void Element::Register()
  {
    switch ( type )
    {
      case Type::Panel:
        PanelRegister();
        break;
      case Type::DataPanel:
        DataPanelRegister();
        break;
      case Type::StackPanel:
        StackPanelRegister();
        break;
      default:
        lookup.emplace( id, std::make_shared<Element>( *this ) );
        break;
    }
  }

  void Element::Disable()
  {
    switch ( type )
    {
      case Type::Panel:
        PanelDisable();
        break;
      case Type::DataPanel:
        DataPanelDisable();
        break;
      case Type::StackPanel:
        StackPanelDisable();
        break;
      default:
        enabled = false;
        break;
    }
  }


  void Element::Reposition()
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
        PanelReposition();
        break;
      case Type::DataPanel:
        DataPanelReposition();
        break;
      case Type::StackPanel:
        StackPanelReposition();
        break;
      default:
        break;
    }
  }


  void Element::Resize()
  {
    if ( !enabled )
      return;

    switch ( type )
    {
      case Type::Panel:
        PanelResize();
        break;
      case Type::DataPanel:
        DataPanelResize();
        break;
      case Type::StackPanel:
        StackPanelResize();
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
        transform.width = texture.width;
        transform.height = texture.height;
      }
      break;
    }
  }

  void Element::LayoutChild(
    Element &child,
    f32 total_children_width,
    f32 &end_of_last_x,
    f32 &end_of_last_y
  )
  {
    if ( children_axis == Axis::Row )
    {
      // 2. Set the child x position based on alignment style.
      switch ( children_horiz_align )
      {
        case Align::Start:
        {
          child.transform.x = end_of_last_x + child.margins.left;
          end_of_last_x =
            child.transform.x + child.transform.width + child.margins.right;
        }
        break;
        case Align::SpaceBetween:
        {
          f32 gap_width = ( this->transform.width - total_children_width ) /
                          ( children.size() - 1 );

          child.transform.x = end_of_last_x + child.margins.left;
          end_of_last_x = child.transform.x + child.transform.width +
                          child.margins.right + gap_width;
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

  void Element::UpdateText( std::string new_text )
  {
    text = new_text;
  }

  void Element::UpdateBackground( Color new_color )
  {
    background = new_color;
  }

  void Element::UpdateClickable( bool new_clickable )
  {
    clickable = new_clickable;
  }

  void Element::ExecuteInterfaceUpdate( const InterfaceUpdate::Update &update )
  {
    switch ( type )
    {
      case Type::Panel:
        PanelExecuteInterfaceUpdate( update );
        break;
      case Type::DataPanel:
        DataPanelExecuteInterfaceUpdate( update );
        break;
      case Type::StackPanel:
        StackPanelExecuteInterfaceUpdate( update );
        break;
      default:
        if ( updates.contains( update.id ) )
          updates[update.id]( *this, update );
        break;
    }
  }

  void Element::Update()
  {
    if ( !enabled )
      return;

    switch ( type )
    {
      case Type::DataPanel:
      {
        if ( self_update )
        {
          self_update( *this );
        }
      }
      break;
      default:
        break;
    }
  }


  void Element::Draw()
  {
    if ( !enabled )
      return;

    switch ( type )
    {
      case Type::Panel:
        PanelDraw();
        break;
      case Type::DataPanel:
        DataPanelDraw();
        break;
      case Type::StackPanel:
        StackPanelDraw();
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
        DrawTextureEx( texture, { transform.x, transform.y }, 0.0, 1, WHITE );
      }
      break;
    }
  }

  void Element::FireEvent()
  {
    if ( on_click )
      InterfaceEvent::event_emitter.publish( *on_click );
  }

};// namespace UI
