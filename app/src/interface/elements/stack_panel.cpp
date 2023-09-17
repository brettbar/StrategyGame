#include "../element.hpp"

namespace UI
{
  void StackPanelElement::Initialize( rect transform )
  {
    Resize( transform );
    Reposition( transform );

    if ( !tabs[curr_index]->starts_disabled )
      tabs[curr_index]->Enable();
  }

  void StackPanelElement::Enable( rect transform )
  {
    Resize( transform );
    Reposition( transform );

    tabs[curr_index]->Enable();
  }

  void StackPanelElement::Register()
  {
    tabs[curr_index]->Register();
  }

  void StackPanelElement::Disable()
  {
    tabs[curr_index]->Disable();
  }

  void StackPanelElement::SwitchChild( u32 index )
  {
    if ( index > tabs.size() )
    {
      assert( false );
      std::cout << "ERROR :: "
                << "StackPanel tried to switch to index greater than its "
                   "number of children."
                << "\n";
      return;
    }
    tabs[curr_index]->Disable();
    curr_index = index;
    tabs[curr_index]->Enable();
  }

  void StackPanelElement::Resize( rect &transform )
  {
    tabs[curr_index]->ResizeRecursive();
    transform.width = tabs[curr_index]->transform.width;
    transform.height = tabs[curr_index]->transform.height;
  }

  void StackPanelElement::Reposition( rect transform )
  {
    tabs[curr_index]->transform.x = transform.x;
    tabs[curr_index]->transform.y = transform.y;
    tabs[curr_index]->RepositionRecursive();
  }

  void StackPanelElement::Draw()
  {
    tabs[curr_index]->Draw();
  }

  void StackPanelElement::ExecuteInterfaceUpdate(
    const InterfaceUpdate::Update &update
  )
  {
    tabs[curr_index]->ExecuteInterfaceUpdate( update );
  }
};// namespace UI