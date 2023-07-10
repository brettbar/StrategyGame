#pragma once

#include "../element.hpp"

namespace UI
{
  void Element::DataPanelEnable()
  {
    assert( type == Type::DataPanel );

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

  void Element::DataPanelRegister()
  {
    assert( type == Type::DataPanel );

    lookup.emplace( id, std::make_shared<Element>( *this ) );

    for ( auto &pair: data_points )
    {
      Element &child = pair.second;
      child.Register();
    }
  }

  void Element::DataPanelDisable()
  {
    assert( type == Type::DataPanel );

    for ( auto &pair: data_points )
    {
      Element &child = pair.second;
      child.Disable();
    }

    enabled = false;
  }
};// namespace UI
