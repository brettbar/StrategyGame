#pragma once

#include "../element.hpp"
#include <raylib.h>

namespace UI
{
  GridPanelElement::Slot GridSlot(
    GridPanelElement::Slot::Dimensions dims,
    sptr<Element> child
  )
  {
    return GridPanelElement::Slot{ dims, child };
  }

  u32 GridPanelElement::GridIndex( u32 col, u32 row )
  {
    return col + (num_cols) *row;
  }

  void GridPanelElement::Initialize( rect transform )
  {
    Resize( transform );
    Reposition( transform );

    for ( GridPanelElement::Slot &slot: filled_slots )
    {
      if ( slot.child->starts_disabled )
        continue;
      slot.child->Enable();
      slot.child->ResizeRecursive();
      slot.child->RepositionRecursive();
    }
  }

  void GridPanelElement::Enable( rect transform )
  {
    Resize( transform );
    Reposition( transform );

    for ( GridPanelElement::Slot &slot: filled_slots )
    {
      slot.child->Enable();
      slot.child->ResizeRecursive();
      slot.child->RepositionRecursive();
    }
  }

  void GridPanelElement::FillNextGridSlot( sptr<Element> new_child )
  {
    u32 num_children = filled_slots.size();
    u32 next_child_index = ( num_rows * num_cols ) - num_children;

    u32 next_col = next_child_index % num_cols;
    u32 next_row = ( next_child_index - next_col ) / num_cols;

    std::cout << "FillNextGridSlot " << next_col << " " << next_row << "\n";

    // TODO account for multiple slot spanning children
    this->filled_slots.push_back(
      GridSlot( { next_col, next_col, next_row, next_row }, new_child )
    );
  }

  void GridPanelElement::Update()
  {
    for ( auto &slot: filled_slots )
    {
      slot.child->Update();
    }
  }

  bool GridPanelElement::AlreadyHasChild( str id )
  {
    map<str, bool> existing_ids = {};
    for ( Slot &slot: filled_slots )
    {
      if ( !existing_ids[slot.child->id] )
      {
        existing_ids.emplace( slot.child->id, true );
      }
    }

    return existing_ids[id];
  }

  void GridPanelElement::Register()
  {

    for ( GridPanelElement::Slot &slot: filled_slots )
    {
      slot.child->Register();
    }
  }

  void GridPanelElement::Disable()
  {
    for ( GridPanelElement::Slot &slot: filled_slots )
    {
      slot.child->Disable();
    }
  }

  void GridPanelElement::Reposition( rect transform )
  {

    u32 slot_width = transform.width / num_cols;
    u32 slot_height = transform.height / num_rows;


    for ( GridPanelElement::Slot &slot: filled_slots )
    {
      slot.child->transform.x = transform.x + slot.dims.start_col * slot_width;
      slot.child->transform.y = transform.y + slot.dims.start_row * slot_height;

      slot.child->RepositionRecursive();
    }
  }

  void GridPanelElement::Resize( rect transform )
  {

    u32 slot_width = transform.width / num_cols;
    u32 slot_height = transform.height / num_rows;

    for ( GridPanelElement::Slot &slot: filled_slots )
    {
      u32 num_wide = slot.dims.end_col - slot.dims.start_col + 1;
      u32 num_tall = slot.dims.end_row - slot.dims.start_row + 1;

      slot.child->transform.width = slot_width * num_wide;
      slot.child->transform.height = slot_height * num_tall;

      slot.child->ResizeRecursive();
    }
  }

  void GridPanelElement::Draw( rect transform )
  {
    u32 slot_width = transform.width / num_cols;
    u32 slot_height = transform.height / num_rows;

    // Draw the grid
    // for ( u32 c = 0; c < num_cols; c++ )
    // {
    //   for ( u32 r = 0; r < num_rows; r++ )
    //   {
    //     DrawRectangleRec(
    //       {
    //         transform.x + (f32) ( c * slot_width ),
    //         transform.y + (f32) ( r * slot_height ),
    //         (f32) slot_width,
    //         (f32) slot_height,
    //       },
    //       GRAY
    //     );
    //     DrawRectangleLinesEx(
    //       {
    //         transform.x + (f32) ( c * slot_width ),
    //         transform.y + (f32) ( r * slot_height ),
    //         (f32) slot_width,
    //         (f32) slot_height,
    //       },
    //       4,
    //       BLACK
    //     );
    //   }
    // }

    for ( GridPanelElement::Slot &slot: filled_slots )
    {
      slot.child->Draw();
    }
  }

  void GridPanelElement::ExecuteInterfaceUpdate(
    const InterfaceUpdate::Update &update
  )
  {
    for ( GridPanelElement::Slot &slot: filled_slots )
    {
      slot.child->ExecuteInterfaceUpdate( update );
    }
  }
};// namespace UI