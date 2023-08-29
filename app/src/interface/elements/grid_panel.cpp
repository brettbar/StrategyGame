#pragma once

#include "../element.hpp"
#include <raylib.h>

namespace UI
{
  u32 GridPanelElement::GridIndex( u32 col, u32 row )
  {
    return col + (num_cols) *row;
  }


  void GridPanelElement::PanelEnable( rect transform )
  {
    PanelResize( transform );
    PanelReposition( transform );

    for ( GridPanelElement::Slot &slot: children )
    {
      slot.child->Enable();
      slot.child->ResizeRecursive();
      slot.child->RepositionRecursive();
    }
  }

  void GridPanelElement::PanelRegister()
  {

    for ( GridPanelElement::Slot &slot: children )
    {
      slot.child->Register();
    }
  }

  void GridPanelElement::PanelDisable()
  {
    for ( GridPanelElement::Slot &slot: children )
    {
      slot.child->Disable();
    }
  }

  void GridPanelElement::PanelReposition( rect transform )
  {

    u32 slot_width = transform.width / num_cols;
    u32 slot_height = transform.height / num_rows;


    for ( GridPanelElement::Slot &slot: children )
    {
      slot.child->transform.x = transform.x + slot.dims.start_col * slot_width;
      slot.child->transform.y = transform.y + slot.dims.start_row * slot_height;

      slot.child->RepositionRecursive();
    }
  }

  void GridPanelElement::PanelResize( rect transform )
  {

    u32 slot_width = transform.width / num_cols;
    u32 slot_height = transform.height / num_rows;

    for ( GridPanelElement::Slot &slot: children )
    {
      u32 num_wide = slot.dims.end_col - slot.dims.start_col + 1;
      u32 num_tall = slot.dims.end_row - slot.dims.start_row + 1;

      slot.child->transform.width = slot_width * num_wide;
      slot.child->transform.height = slot_height * num_tall;

      slot.child->ResizeRecursive();
    }
  }

  void GridPanelElement::PanelDraw( rect transform )
  {
    u32 slot_width = transform.width / num_cols;
    u32 slot_height = transform.height / num_rows;

    // Draw the grid
    for ( u32 c = 0; c < num_cols; c++ )
    {
      for ( u32 r = 0; r < num_rows; r++ )
      {
        DrawRectangleRec(
          {
            transform.x + (f32) ( c * slot_width ),
            transform.y + (f32) ( r * slot_height ),
            (f32) slot_width,
            (f32) slot_height,
          },
          GRAY
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

    for ( GridPanelElement::Slot &slot: children )
    {
      slot.child->Draw();
    }
  }

  void GridPanelElement::PanelExecuteInterfaceUpdate(
    const InterfaceUpdate::Update &update
  )
  {
    for ( GridPanelElement::Slot &slot: children )
    {
      slot.child->ExecuteInterfaceUpdate( update );
    }
  }


};// namespace UI