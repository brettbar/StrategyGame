#include "../element.hpp"

namespace UI
{
  void Element::StackPanelEnable()
  {
    assert( type == Type::StackPanel );

    enabled = true;
    Resize();
    Reposition();

    children[curr_index].Enable();
    children[curr_index].Resize();
    children[curr_index].Reposition();
  }

  void Element::StackPanelRegister()
  {
    assert( type == Type::StackPanel );

    lookup.emplace( id, std::make_shared<Element>( *this ) );
    children[curr_index].Register();
  }

  void Element::StackPanelDisable()
  {
    assert( type == Type::StackPanel );
    children[curr_index].Disable();
    enabled = false;
  }

  void Element::StackPanelSwitchChild( u32 index )
  {
    assert( type == Type::StackPanel );

    if ( index > children.size() )
    {
      assert( false );
      std::cout << "ERROR :: "
                << "StackPanel tried to switch to index greater than its "
                   "number of children."
                << "\n";
      return;
    }
    children[curr_index].Disable();
    curr_index = index;
    children[curr_index].Enable();
  }
};// namespace UI