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

};// namespace UI