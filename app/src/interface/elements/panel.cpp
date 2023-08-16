#pragma once

#include "../element.hpp"
#include <raylib.h>

namespace UI
{
  u32 GridPanelElement::GridIndex( u32 col, u32 row )
  {
    return col + (num_cols) *row;
  }


  void Element::PanelEnable()
  {
    assert( type == Type::GridPanel );
    enabled = true;
    PanelResize();
    RepositionRecursive();

    for ( GridPanelElement::Slot &slot: grid_panel->children )
    {
      slot.child->Enable();
      slot.child->ResizeRecursive();
      slot.child->RepositionRecursive();
    }
  }

  void Element::PanelRegister()
  {
    assert( type == Type::GridPanel );
    lookup.emplace( id, std::make_shared<Element>( *this ) );

    for ( GridPanelElement::Slot &slot: grid_panel->children )
    {
      slot.child->Register();
    }
  }

  void Element::PanelDisable()
  {
    assert( type == Type::GridPanel );

    for ( GridPanelElement::Slot &slot: grid_panel->children )
    {
      slot.child->Disable();
    }

    enabled = false;
  }

  void Element::PanelReposition()
  {
    assert( type == Type::GridPanel );

    u32 slot_width = transform.width / grid_panel->num_cols;
    u32 slot_height = transform.height / grid_panel->num_rows;


    for ( GridPanelElement::Slot &slot: grid_panel->children )
    {
      slot.child->transform.x = transform.x + slot.dims.start_col * slot_width;
      slot.child->transform.y = transform.y + slot.dims.start_row * slot_height;

      slot.child->RepositionRecursive();
    }
  }

  void Element::PanelResize()
  {
    assert( type == Type::GridPanel );

    u32 slot_width = transform.width / grid_panel->num_cols;
    u32 slot_height = transform.height / grid_panel->num_rows;

    for ( GridPanelElement::Slot &slot: grid_panel->children )
    {
      u32 num_wide = slot.dims.end_col - slot.dims.start_col + 1;
      u32 num_tall = slot.dims.end_row - slot.dims.start_row + 1;

      slot.child->transform.width = slot_width * num_wide;
      slot.child->transform.height = slot_height * num_tall;

      slot.child->ResizeRecursive();
    }
  }

  void Element::PanelDraw()
  {
    assert( type == Type::GridPanel );

    DrawRectangleRec( transform, background );

    u32 slot_width = transform.width / grid_panel->num_cols;
    u32 slot_height = transform.height / grid_panel->num_rows;

    for ( u32 c = 0; c < grid_panel->num_cols; c++ )
    {
      for ( u32 r = 0; r < grid_panel->num_rows; r++ )
      {
        DrawRectangleRec(
          {
            transform.x + (f32) ( c * slot_width ),
            transform.y + (f32) ( r * slot_height ),
            (f32) slot_width,
            (f32) slot_height,
          },
          background
        );
        DrawRectangleLinesEx(
          {
            transform.x + (f32) ( c * slot_width ),
            transform.y + (f32) ( r * slot_height ),
            (f32) slot_width,
            (f32) slot_height,
          },
          4,
          BLACK
        );
      }
    }

    for ( GridPanelElement::Slot &slot: grid_panel->children )
    {
      slot.child->Draw();
    }
  }

  void Element::PanelExecuteInterfaceUpdate(
    const InterfaceUpdate::Update &update
  )
  {
    assert( type == Type::GridPanel );
    if ( updates.contains( update.id ) )
      updates[update.id]( *this, update );

    for ( GridPanelElement::Slot &slot: grid_panel->children )
    {
      slot.child->ExecuteInterfaceUpdate( update );
    }
  }


};// namespace UI