#include "element.hpp"
#include <raylib.h>

namespace UI
{
  void Element::Register()
  {
    lookup.emplace( id, std::make_shared<Element>( *this ) );

    switch ( type )
    {
      case Type::GridPanel:
        grid_panel->Register();
        break;
      case Type::DataPanel:
        // DataPanelRegister();
        break;
      case Type::StackPanel:
        stack_panel->Register();
        break;
    }
  }

  void Element::Initialize()
  {
    if ( starts_disabled )
      return;

    enabled = true;

    switch ( type )
    {
      case Type::GridPanel:
        grid_panel->Initialize( transform );
        break;
      case Type::DataPanel:
        // DataPanelEnable();
        break;
      case Type::StackPanel:
        stack_panel->Initialize( transform );
        break;
    }
  }

  void Element::Enable()
  {
    enabled = true;
    switch ( type )
    {
      case Type::GridPanel:
        grid_panel->Enable( transform );
        break;
      case Type::DataPanel:
        // DataPanelEnable();
        break;
      case Type::StackPanel:
        stack_panel->Enable( transform );
        break;
    }
  }

  void Element::Disable()
  {
    switch ( type )
    {
      case Type::GridPanel:
        grid_panel->Disable();
        break;
      case Type::DataPanel:
        // DataPanelDisable();
        break;
      case Type::StackPanel:
        stack_panel->Disable();
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
        grid_panel->Resize( transform );
        break;
      case Type::DataPanel:
        // DataPanelResize();
        break;
      case Type::StackPanel:
        stack_panel->Resize( transform );
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
        grid_panel->Reposition( transform );
        break;
      case Type::DataPanel:
        // DataPanelReposition();
        break;
      case Type::StackPanel:
        stack_panel->Reposition( transform );
        break;
    }
  }

  void Element::UpdateText( std::string new_text )
  {

    switch ( type )
    {
      case Type::TextButton:
        text_button->label->text = new_text;
        break;
      case Type::TextLabel:
        text_label->text = new_text;
        break;
    }
  }

  void Element::UpdateBackground( Color new_color )
  {
    background = new_color;
  }

  void Element::UpdateClickable( bool new_clickable )
  {
    switch ( type )
    {
      case Type::TextButton:
        text_button->clickable = new_clickable;
        break;
      case Type::TextureButton:
        texture_button->clickable = new_clickable;
        break;
    }
  }

  void Element::ExecuteInterfaceUpdate( const InterfaceUpdate::Update &update )
  {
    if ( updates.contains( update.id ) )
      updates[update.id]( *this, update );

    switch ( type )
    {
      case Type::GridPanel:
        grid_panel->ExecuteInterfaceUpdate( update );
        break;
      case Type::DataPanel:
        // DataPanelExecuteInterfaceUpdate( update );
        break;
      case Type::StackPanel:
        stack_panel->ExecuteInterfaceUpdate( update );
        break;
    }
  }

  void Element::Update()
  {
    if ( !enabled )
      return;

    switch ( type )
    {
      case Type::GridPanel:
      {
        grid_panel->Update();
      }
      break;
      case Type::DataPanel:
      {
        if ( self_update )
        {
          self_update( *this );
        }
      }
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
        DrawRectangleRec( transform, background );
        grid_panel->Draw( transform );
        break;
      case Type::DataPanel:
        // DataPanelDraw();
        break;
      case Type::StackPanel:
        DrawRectangleRec( transform, background );
        stack_panel->Draw();
        break;
      case Type::TextLabel:
        DrawRectangleRec( transform, background );
        text_label->Draw( background, transform );
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
        DrawRectangleRec( transform, background );
        text_button->Draw( background, transform );
      }
      break;
      case Type::TextureLabel:
      {
        DrawRectangleRec( transform, background );
        DrawTextureEx(
          texture_label->texture, { transform.x, transform.y }, 0.0, 1, WHITE
        );
      }
      break;
      case Type::TextureButton:
      {
        DrawRectangleRec( transform, background );
        DrawTextureEx(
          texture_button->label->texture,
          { transform.x, transform.y },
          0.0,
          1,
          WHITE
        );
      }
      break;
    }
  }

  // X2749B
  void Element::FireEvent()
  {
    switch ( type )
    {
      case ( Type::TextButton ):
      {
        if ( text_button->on_click )
        {
          InterfaceEvent::event_emitter.publish( *text_button->on_click );
        }
      }
      break;
      case ( Type::TextureButton ):
      {
        if ( texture_button->on_click )
        {
          InterfaceEvent::event_emitter.publish( *texture_button->on_click );
        }
      }
      break;
    }
  }

};// namespace UI
