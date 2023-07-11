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

    Update();

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
  }

};// namespace UI