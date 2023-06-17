#include "element.hpp"

namespace UI
{
  void Element::InitialEnable()
  {
    if ( starts_disabled )
      return;

    Enable();
  }

  void Element::Enable()
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

  void Element::Register()
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

  void Element::Disable()
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

  void Element::SwitchChild( u32 index )
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

  void Element::Resize()
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
        transform.width = texture.width * UI::SCALE * 2;
        transform.height = texture.height * UI::SCALE * 2;
      }
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

  void Element::UpdateChildren()
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
        if ( update )
        {
          update( *this );
        }
      }
      break;
      default:
        break;
    }
  }

  void Element::CreateElementForDatapoints( Element element )
  {
    if ( !data_points.contains( element.id ) )
    {
      element.Enable();
      data_points.insert_or_assign( element.id, element );
    }
    else
    {
    }
  }

  void Element::Draw()
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
          texture, { transform.x, transform.y }, 0.0, SCALE * 2, WHITE
        );
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
