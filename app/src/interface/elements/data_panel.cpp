#pragma once

#include "../element.hpp"

namespace UI
{
  // void Element::DataPanelEnable()
  // {
  //   assert( type == Type::DataPanel );

  //   enabled = true;
  //   ResizeRecursive();
  //   RepositionRecursive();

  //   for ( auto &pair: data_points )
  //   {
  //     Element &child = pair.second;
  //     child.Enable();
  //     child.ResizeRecursive();
  //     child.RepositionRecursive();
  //   }
  // }

  // void Element::DataPanelRegister()
  // {
  //   assert( type == Type::DataPanel );

  //   lookup.emplace( id, std::make_shared<Element>( *this ) );

  //   for ( auto &pair: data_points )
  //   {
  //     Element &child = pair.second;
  //     child.Register();
  //   }
  // }

  // void Element::DataPanelDisable()
  // {
  //   assert( type == Type::DataPanel );

  //   for ( auto &pair: data_points )
  //   {
  //     Element &child = pair.second;
  //     child.Disable();
  //   }

  //   enabled = false;
  // }


  // void Element::DataPanelResize()
  // {
  //   // assert( type == Type::DataPanel );

  //   // f32 total_height = 0;
  //   // f32 total_width = 0;
  //   // f32 tallest_child = 0;
  //   // f32 widest_child = 0;

  //   // Update();

  //   // for ( auto &pair: data_points )
  //   // {
  //   //   Element &child = pair.second;
  //   //   // TODO not sure if this is right
  //   //   if ( !child.enabled )
  //   //     continue;

  //   //   child.ResizeRecursive();
  //   // }

  //   // for ( auto &pair: data_points )
  //   // {
  //   //   Element &child = pair.second;
  //   //   if ( !child.enabled )
  //   //     continue;

  //   //   total_width += child.transform.width;
  //   //   total_height += child.transform.height;

  //   //   if ( child.transform.width > widest_child )
  //   //     widest_child = child.transform.width;

  //   //   if ( child.transform.height > tallest_child )
  //   //     tallest_child = child.transform.height;
  //   // }

  //   // if ( size == Size::Minimum )
  //   // {
  //   //   if ( children_axis == Axis::Row )
  //   //   {
  //   //     transform.width = total_width;
  //   //     transform.height = tallest_child;
  //   //   }
  //   //   else
  //   //   {
  //   //     transform.width = widest_child;
  //   //     transform.height = total_height;
  //   //   }
  //   // }
  // }

  // void Element::DataPanelReposition()
  // {
  //   // assert( type == Type::DataPanel );

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
  //   //       for ( auto &pair: data_points )
  //   //       {
  //   //         Element &child = pair.second;
  //   //         child.RepositionRecursive();

  //   //         child.transform.x = end_of_last_x + child.margins.left;
  //   //         end_of_last_x =
  //   //           child.transform.x + child.transform.width + child.margins.right;
  //   //       }
  //   //     }
  //   //     break;
  //   //     case Align::SpaceBetween:
  //   //     {
  //   //       for ( auto &pair: data_points )
  //   //       {
  //   //         Element &child = pair.second;
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
  //   //       for ( auto &pair: data_points )
  //   //       {
  //   //         Element &child = pair.second;
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
  //   //       for ( auto &pair: data_points )
  //   //       {
  //   //         Element &child = pair.second;
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
  //   //       for ( auto &pair: data_points )
  //   //       {
  //   //         Element &child = pair.second;
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

  // void Element::DataPanelDraw()
  // {
  //   assert( type == Type::DataPanel );

  //   DrawRectangleV(
  //     { transform.x, transform.y },
  //     { transform.width, transform.height },
  //     background
  //   );

  //   for ( auto &pair: data_points )
  //   {
  //     Element &child = pair.second;
  //     child.Draw();
  //   }
  // }

  // void Element::DataPanelExecuteInterfaceUpdate(
  //   const InterfaceUpdate::Update &update
  // )
  // {
  //   assert( type == Type::DataPanel );

  //   if ( updates.contains( update.id ) )
  //     updates[update.id]( *this, update );

  //   for ( auto &pair: data_points )
  //   {
  //     Element &child = pair.second;
  //     child.ExecuteInterfaceUpdate( update );
  //   }
  // }

  // void Element::CreateElementForDatapoints( Element element )
  // {
  //   if ( !data_points.contains( element.id ) )
  //   {
  //     element.Enable();
  //     data_points.insert_or_assign( element.id, element );
  //   }
  //   else
  //   {
  //   }
  // }
};// namespace UI
