#pragma once

#include "../element.hpp"
#include <raylib.h>

namespace UI
{
  void Panel::PanelEnable( Element &self )
  {
    self.enabled = true;
    PanelResize( self );
    // RepositionRecursive();

    for ( Element *child: children )
    {
      child->Enable();
      child->ResizeRecursive();
      child->RepositionRecursive();
    }
  }

  void Panel::PanelRegister()
  {
    for ( Element *child: children )
    {
      child->Register();
    }
  }

  void Panel::PanelDisable()
  {
    for ( Element *child: children )
    {
      child->Disable();
    }
  }

  void Panel::PanelResize( const Element &self )
  {
    u32 slot_width = self.transform.width / num_cols;
    u32 slot_height = self.transform.height / num_rows;

    // resize grid
    for ( u32 c = 0; c < num_cols; c++ )
    {
      for ( u32 r = 0; r < num_rows; r++ )
      {
        grid[c + num_cols * r] = {
          self.transform.x + (f32) ( c * slot_width ),
          self.transform.y + (f32) ( r * slot_height ),
          (f32) slot_width,
          (f32) slot_height,
        };
      }
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

  void Panel::PanelReposition()
  {
    // assert( type == Type::Panel );
    // f32 total_height = 0;
    // f32 total_width = 0;
    // f32 tallest_child = 0;
    // f32 widest_child = 0;
    // f32 end_of_last_x = transform.x;
    // f32 end_of_last_y = transform.y;

    // if ( children_axis == Axis::Row )
    // {
    //   switch ( children_horiz_align )
    //   {
    //     case Align::Start:
    //     {
    //       for ( Element &child: children )
    //       {
    //         child.RepositionRecursive();

    //         child.transform.x = end_of_last_x + child.margins.left;
    //         end_of_last_x =
    //           child.transform.x + child.transform.width + child.margins.right;
    //       }
    //     }
    //     break;
    //     case Align::SpaceBetween:
    //     {
    //       for ( Element &child: children )
    //       {
    //         child.RepositionRecursive();

    //         f32 gap_width =
    //           ( this->transform.width - total_width ) / ( children.size() - 1 );

    //         child.transform.x = end_of_last_x + child.margins.left;
    //         end_of_last_x = child.transform.x + child.transform.width +
    //                         child.margins.right + gap_width;
    //       }
    //     }
    //     break;
    //   }

    //   switch ( children_vert_align )
    //   {
    //     case Align::Start:
    //     {
    //       for ( Element &child: children )
    //       {
    //         child.RepositionRecursive();
    //         child.transform.y = transform.y;
    //       }
    //     }
    //     break;
    //   }
    // }
    // else
    // {// Axis::Column
    //   // 2. Set the child x position based on alignment style.
    //   switch ( children_horiz_align )
    //   {
    //     case Align::Start:
    //     {
    //       for ( Element &child: children )
    //       {
    //         child.RepositionRecursive();
    //         child.transform.x = transform.x;
    //       }
    //     }
    //     break;
    //   }

    //   // 3. Set the child y position based on alignment style.
    //   switch ( children_vert_align )
    //   {
    //     case Align::Start:
    //     {
    //       for ( Element &child: children )
    //       {
    //         child.RepositionRecursive();
    //         child.transform.y = end_of_last_y;
    //         // + margins.top;
    //         end_of_last_y = child.transform.y + child.transform.height;
    //         // + margins.bottom;
    //       }
    //     }
    //     break;
    //   }
    // }
  }


  void Panel::PanelDraw( const Element &self )
  {
    for ( rect slot: grid )
    {
      DrawRectangleRec( slot, GRAY );
    }

    DrawRectangleRec( self.transform, self.background );

    for ( Element *child: children )
    {
      child->Draw();
    }
  }


  void Panel::PanelExecuteInterfaceUpdate(
    const Element &self,
    const InterfaceUpdate::Update &update
  )
  {
    // if ( self.updates.contains( update.id ) )
    //   self.updates[update.id]( self, update );

    // for ( auto &child: children )
    // {
    //   child.ExecuteInterfaceUpdate( update );
    // }
  }
};// namespace UI