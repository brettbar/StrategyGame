#include "../element.hpp"

namespace UI
{
  // void Element::StackPanelEnable()
  // {
  //   assert( type == Type::StackPanel );

  //   enabled = true;
  //   ResizeRecursive();
  //   RepositionRecursive();

  //   children[curr_index].Enable();
  //   children[curr_index].ResizeRecursive();
  //   children[curr_index].RepositionRecursive();
  // }

  // void Element::StackPanelRegister()
  // {
  //   assert( type == Type::StackPanel );

  //   lookup.emplace( id, std::make_shared<Element>( *this ) );
  //   children[curr_index].Register();
  // }

  // void Element::StackPanelDisable()
  // {
  //   assert( type == Type::StackPanel );
  //   children[curr_index].Disable();
  //   enabled = false;
  // }

  // void Element::StackPanelSwitchChild( u32 index )
  // {
  //   assert( type == Type::StackPanel );

  //   if ( index > children.size() )
  //   {
  //     assert( false );
  //     std::cout << "ERROR :: "
  //               << "StackPanel tried to switch to index greater than its "
  //                  "number of children."
  //               << "\n";
  //     return;
  //   }
  //   children[curr_index].Disable();
  //   curr_index = index;
  //   children[curr_index].Enable();
  // }

  // void Element::StackPanelResize()
  // {
  //   assert( type == Type::StackPanel );

  //   children[curr_index].ResizeRecursive();
  //   transform.width = children[curr_index].transform.width;
  //   transform.height = children[curr_index].transform.height;
  // }

  // void Element::StackPanelReposition()
  // {
  //   assert( type == Type::StackPanel );
  //   children[curr_index].transform.x = transform.x;
  //   children[curr_index].transform.y = transform.y;
  //   children[curr_index].RepositionRecursive();
  // }

  // void Element::StackPanelDraw()
  // {
  //   DrawRectangleV(
  //     { transform.x, transform.y },
  //     { transform.width, transform.height },
  //     background
  //   );

  //   children[curr_index].Draw();
  // }

  // void Element::StackPanelExecuteInterfaceUpdate(
  //   const InterfaceUpdate::Update &update
  // )
  // {
  //   assert( type == Type::StackPanel );

  //   if ( updates.contains( update.id ) )
  //     updates[update.id]( *this, update );

  //   children[curr_index].ExecuteInterfaceUpdate( update );
  // }
};// namespace UI