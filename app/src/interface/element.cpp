#include "element.hpp"

namespace UI
{
  void Element::Enable()
  {
    switch ( type )
    {
      case Type::GridPanel:
        panel->PanelEnable( *this );
        break;
      case Type::DataPanel:
        // DataPanelEnable();
        break;
      case Type::StackPanel:
        // StackPanelEnable();
        break;
      default:
        enabled = true;
        break;
    }
  }

  void Element::Register()
  {
    lookup.emplace( id, std::make_shared<Element>( *this ) );

    switch ( type )
    {
      case Type::GridPanel:
        panel->PanelRegister();
        break;
      case Type::DataPanel:
        // DataPanelRegister();
        break;
      case Type::StackPanel:
        // StackPanelRegister();
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
      case Type::GridPanel:
        panel->PanelDisable();
        break;
      case Type::DataPanel:
        // DataPanelDisable();
        break;
      case Type::StackPanel:
        // StackPanelDisable();
        break;
      default:
        break;
    }

    enabled = false;
  }

  // TODO This should only be called for Panels
  void Element::ResizeRecursive()
  {
    bool is_panel = type == Type::GridPanel || type == Type::DataPanel ||
                    type == Type::StackPanel;

    if ( !is_panel )
      return;

    switch ( type )
    {
      case Type::GridPanel:
        panel->PanelResize( *this );
        break;
      case Type::DataPanel:
        // DataPanelResize();
        break;
      case Type::StackPanel:
        // StackPanelResize();
        break;
      default:
        break;
    }
  }


  // TODO This should only be called for Panels
  void Element::RepositionRecursive()
  {
    bool is_panel = type == Type::GridPanel || type == Type::DataPanel ||
                    type == Type::StackPanel;

    if ( !is_panel )
      return;


    switch ( type )
    {
      case Type::GridPanel:
        panel->PanelReposition();
        break;
      case Type::DataPanel:
        // DataPanelReposition();
        break;
      case Type::StackPanel:
        // StackPanelReposition();
        break;
      default:
        break;
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
      case Type::GridPanel:
        panel->PanelExecuteInterfaceUpdate( *this, update );
        break;
      case Type::DataPanel:
        // DataPanelExecuteInterfaceUpdate( update );
        break;
      case Type::StackPanel:
        // StackPanelExecuteInterfaceUpdate( update );
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
      case Type::GridPanel:
        panel->PanelDraw( *this );
        break;
      case Type::DataPanel:
        // DataPanelDraw();
        break;
      case Type::StackPanel:
        // StackPanelDraw();
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
