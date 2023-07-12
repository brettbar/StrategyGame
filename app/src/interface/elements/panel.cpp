#pragma once

#include "../element.hpp"

namespace UI
{
  void Element::PanelEnable()
  {
    assert( type == Type::Panel );
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

  void Element::PanelRegister()
  {
    assert( type == Type::Panel );
    lookup.emplace( id, std::make_shared<Element>( *this ) );

    for ( Element &child: children )
    {
      child.Register();
    }
  }

  void Element::PanelDisable()
  {
    assert( type == Type::Panel );

    for ( Element &child: children )
    {
      child.Disable();
    }

    enabled = false;
  }

  void Element::PanelResize()
  {
    assert( type == Type::Panel );

    f32 total_height = 0;
    f32 total_width = 0;
    f32 tallest_child = 0;
    f32 widest_child = 0;

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

    if ( size == Size::Minimum )
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
  }

  void Element::PanelReposition()
  {
    assert( type == Type::Panel );
    f32 total_height = 0;
    f32 total_width = 0;
    f32 tallest_child = 0;
    f32 widest_child = 0;
    f32 end_of_last_x = transform.x;
    f32 end_of_last_y = transform.y;

    if ( children_axis == Axis::Row )
    {
      switch ( children_horiz_align )
      {
        case Align::Start:
        {
          for ( Element &child: children )
          {
            child.Reposition();

            child.transform.x = end_of_last_x + child.margins.left;
            end_of_last_x =
              child.transform.x + child.transform.width + child.margins.right;
          }
        }
        break;
        case Align::SpaceBetween:
        {
          for ( Element &child: children )
          {
            child.Reposition();
            f32 gap_width =
              ( this->transform.width - total_width ) / ( children.size() - 1 );

            child.transform.x = end_of_last_x + child.margins.left;
            end_of_last_x = child.transform.x + child.transform.width +
                            child.margins.right + gap_width;
          }
        }
        break;
      }

      switch ( children_vert_align )
      {
        case Align::Start:
        {
          for ( Element &child: children )
          {
            child.Reposition();
            child.transform.y = transform.y;
          }
        }
        break;
      }
    }
    else
    {// Axis::Column
      // 2. Set the child x position based on alignment style.
      switch ( children_horiz_align )
      {
        case Align::Start:
        {
          for ( Element &child: children )
          {
            child.Reposition();
            child.transform.x = transform.x;
          }
        }
        break;
      }

      // 3. Set the child y position based on alignment style.
      switch ( children_vert_align )
      {
        case Align::Start:
        {
          for ( Element &child: children )
          {
            child.Reposition();
            child.transform.y = end_of_last_y;
            // + margins.top;
            end_of_last_y = child.transform.y + child.transform.height;
            // + margins.bottom;
          }
        }
        break;
      }
    }
  }


  void Element::PanelDraw()
  {
    assert( type == Type::Panel );

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


  void Element::PanelExecuteInterfaceUpdate(
    const InterfaceUpdate::Update &update
  )
  {
    assert( type == Type::Panel );
    if ( updates.contains( update.id ) )
      updates[update.id]( *this, update );

    for ( auto &child: children )
    {
      child.ExecuteInterfaceUpdate( update );
    }
  }
};// namespace UI