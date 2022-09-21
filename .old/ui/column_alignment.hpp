#pragma once

#include "../../src/ui/ui_components.hpp"

namespace UI {
inline void Col_Align( entt::registry &, Panel & );

inline void
Col_HorizStart( entt::registry &, f32, f32, std::vector<entt::entity> );

inline void
Col_HorizSpaceOut( entt::registry &, f32, f32, std::vector<entt::entity> );

inline void
Col_VertStart( entt::registry &, f32, f32, std::vector<entt::entity> );

inline void
Col_VertSpaceOut( entt::registry &, f32, f32, f32, std::vector<entt::entity> );


inline void Col_Align( entt::registry &reg, entt::entity panel_entity ) {
  Element &panel_elem = reg.get<Element>( panel_entity );
  Panel &panel = reg.get<Panel>( panel_entity );

  f32 total_height = 0;
  f32 widest_child = 0;

  for ( u32 i = 0; i < panel.children.size(); i++ ) {
    entt::entity &child = panel.children[i];
    rect &elem = ui_reg.get<Element>( child ).transform;

    // if (
    //   elem.dimensions_type == Dimensions::FIX_WIDTH_STRETCH_HEIGHT ||
    //   elem.dimensions_type == Dimensions::STRETCH ) {
    //   elem.height = parent_height / children.size();
    // }

    total_height += elem.height;

    if ( elem.width > widest_child )
      widest_child = elem.width;
  }


  // if ( !panel.self_transformed ) {
  //   panel_elem.transform.height = total_height;
  //   panel_elem.transform.width = widest_child;
  // }


  switch ( panel.children_horiz_align ) {
    case Align::START:
      Col_HorizStart(
        reg,
        panel_elem.transform.x,
        panel_elem.transform.width,
        panel.children );
      break;
    case Align::SPACE_OUT:
      Col_HorizSpaceOut(
        reg,
        panel_elem.transform.x,
        panel_elem.transform.width,
        panel.children );
      break;
  }

  switch ( panel.children_vert_align ) {
    case Align::START:
      Col_VertStart(
        reg,
        panel_elem.transform.y,
        panel_elem.transform.height,
        panel.children );
      break;
    case Align::SPACE_OUT:
      Col_VertSpaceOut(
        reg,
        total_height,
        panel_elem.transform.y,
        panel_elem.transform.height,
        panel.children );
      break;
  }
}

inline void Col_HorizStart(
  entt::registry &reg,
  f32 parent_x,
  f32 parent_width,
  std::vector<entt::entity> children ) {
  //
  for ( u32 i = 0; i < children.size(); i++ ) {
    entt::entity &child = children[i];
    rect &elem = ui_reg.get<Element>( child ).transform;

    // if (
    //   elem.dimensions_type == Dimensions::FIX_HEIGHT_STRETCH_WIDTH ||
    //   elem.dimensions_type == Dimensions::STRETCH ) {
    //   elem.width = parent_width;
    // }

    elem.x = parent_x;
  }
}

inline void Col_HorizSpaceOut(
  entt::registry &reg,
  f32 parent_x,
  f32 parent_width,
  std::vector<entt::entity> children ) {}

inline void Col_VertStart(
  entt::registry &reg,
  f32 parent_y,
  f32 parent_height,
  std::vector<entt::entity> children ) {
  f32 end_of_last = parent_y;

  for ( u32 i = 0; i < children.size(); i++ ) {
    entt::entity &child = children[i];
    Element &elem = ui_reg.get<Element>( child );

    // elem.y = end_of_last + elem.margins.top;
    elem.transform.y = end_of_last + elem.margins.top;
    // end_of_last = elem.y + elem.dmns.y + elem.margins.bottom;


    // if ( elem.dimensions_type == Dimensions::FIX_WIDTH_STRETCH_HEIGHT ) {
    //   elem.height = parent_height / children.size();
    // }


    end_of_last =
      elem.transform.y + elem.transform.height + elem.margins.bottom;
  }
}


inline void Col_VertSpaceOut(
  entt::registry &reg,
  f32 total_height,
  f32 parent_y,
  f32 parent_height,
  std::vector<entt::entity> children ) {
  f32 total_gap_height = parent_height - total_height;
  u32 num_gaps = children.size() - 1;

  f32 gap_height = total_gap_height / num_gaps;

  f32 end_of_last = parent_y;
  for ( u32 i = 0; i < children.size(); i++ ) {
    entt::entity &child = children[i];
    rect &elem = ui_reg.get<Element>( child ).transform;

    if ( i % 2 == 0 ) {
      elem.y = end_of_last;
      end_of_last = elem.y + elem.height;
    } else {
      elem.y = end_of_last + gap_height;
      end_of_last = elem.y + elem.height + gap_height;
    }
  }
}

};// namespace UI
