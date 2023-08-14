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
      slot.child->transform.x = slot.dims.start_col * slot_width;
      slot.child->transform.y = slot.dims.start_row * slot_height;

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


  // void Element::OldPanelResize()
  // {
  //   assert( type == Type::GridPanel );

  //   f32 total_height = 0;
  //   f32 total_width = 0;
  //   f32 tallest_child = 0;
  //   f32 widest_child = 0;

  //   for ( Element &child: children )
  //   {
  //     if ( !child.enabled )
  //       continue;

  //     // First, do all the Fixed and Minimal Resizing
  //     switch ( child.type )
  //     {
  //       case Type::GridPanel:
  //         child.PanelResize();
  //         break;
  //       case Type::DataPanel:
  //         child.DataPanelResize();
  //         break;
  //       case Type::StackPanel:
  //         child.StackPanelResize();
  //         break;
  //       case Type::TextButton:
  //       case Type::TextLabel:
  //       {
  //         const vec2 text_dims = MeasureTextEx(
  //           Global::font_cache[hstr{ "font_romulus" }]->font,
  //           child.text.c_str(),
  //           child.font_size,
  //           2.0f
  //         );

  //         child.transform.width = text_dims.x;
  //         child.transform.height = text_dims.y;
  //       }
  //       break;
  //       case Type::TextureButton:
  //       case Type::TextureLabel:
  //       {
  //         child.transform.width = child.texture.width;
  //         child.transform.height = child.texture.height;
  //       }
  //       break;
  //     }

  //     total_width += child.transform.width;
  //     total_height += child.transform.height;

  //     if ( child.transform.width > widest_child )
  //       widest_child = child.transform.width;

  //     if ( child.transform.height > tallest_child )
  //       tallest_child = child.transform.height;
  //   }
  // }

  // void Element::PanelReposition()
  // {
  //   // assert( type == Type::Panel );
  //   // f32 total_height = 0;
  //   // f32 total_width = 0;
  //   // f32 tallest_child = 0;
  //   // f32 widest_child = 0;
  //   // f32 end_of_last_x = transform.x;
  //   // f32 end_of_last_y = transform.y;

  //   // if ( children_axis == Axis::Row )
  //   // {
  //   //   switch ( children_horiz_align )
  //   //   {
  //   //     case Align::Start:
  //   //     {
  //   //       for ( Element &child: children )
  //   //       {
  //   //         child.RepositionRecursive();

  //   //         child.transform.x = end_of_last_x + child.margins.left;
  //   //         end_of_last_x =
  //   //           child.transform.x + child.transform.width + child.margins.right;
  //   //       }
  //   //     }
  //   //     break;
  //   //     case Align::SpaceBetween:
  //   //     {
  //   //       for ( Element &child: children )
  //   //       {
  //   //         child.RepositionRecursive();

  //   //         f32 gap_width =
  //   //           ( this->transform.width - total_width ) / ( children.size() - 1 );

  //   //         child.transform.x = end_of_last_x + child.margins.left;
  //   //         end_of_last_x = child.transform.x + child.transform.width +
  //   //                         child.margins.right + gap_width;
  //   //       }
  //   //     }
  //   //     break;
  //   //   }

  //   //   switch ( children_vert_align )
  //   //   {
  //   //     case Align::Start:
  //   //     {
  //   //       for ( Element &child: children )
  //   //       {
  //   //         child.RepositionRecursive();
  //   //         child.transform.y = transform.y;
  //   //       }
  //   //     }
  //   //     break;
  //   //   }
  //   // }
  //   // else
  //   // {// Axis::Column
  //   //   // 2. Set the child x position based on alignment style.
  //   //   switch ( children_horiz_align )
  //   //   {
  //   //     case Align::Start:
  //   //     {
  //   //       for ( Element &child: children )
  //   //       {
  //   //         child.RepositionRecursive();
  //   //         child.transform.x = transform.x;
  //   //       }
  //   //     }
  //   //     break;
  //   //   }

  //   //   // 3. Set the child y position based on alignment style.
  //   //   switch ( children_vert_align )
  //   //   {
  //   //     case Align::Start:
  //   //     {
  //   //       for ( Element &child: children )
  //   //       {
  //   //         child.RepositionRecursive();
  //   //         child.transform.y = end_of_last_y;
  //   //         // + margins.top;
  //   //         end_of_last_y = child.transform.y + child.transform.height;
  //   //         // + margins.bottom;
  //   //       }
  //   //     }
  //   //     break;
  //   //   }
  //   // }
  // }


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