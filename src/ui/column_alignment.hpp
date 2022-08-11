#pragma once

#include "ui_components.hpp"

namespace UI {
inline void Col_Align( entt::registry &, Panel &, Element & );

inline void
Col_HorizStart( entt::registry &, f32, f32, std::vector<entt::entity> );

inline void
Col_HorizSpaceOut( entt::registry &, f32, f32, std::vector<entt::entity> );

inline void
Col_VertStart( entt::registry &, f32, f32, std::vector<entt::entity> );

inline void
Col_VertSpaceOut( entt::registry &, f32, f32, std::vector<entt::entity> );


inline void Col_Align( entt::registry &reg, Panel &panel, Element &elem ) {
  switch ( panel.children_horiz_align ) {
    case Align::START:
      Col_HorizStart( reg, elem.pos.x, elem.width, panel.children );
      break;
    case Align::SPACE_OUT:
      Col_HorizSpaceOut( reg, elem.pos.x, elem.width, panel.children );
      break;
  }

  switch ( panel.children_vert_align ) {
    case Align::START:
      Col_VertStart( reg, elem.pos.y, elem.height, panel.children );
      break;
    case Align::SPACE_OUT:
      Col_VertSpaceOut( reg, elem.pos.y, elem.height, panel.children );
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
    entt::entity &entity = children[i];
    Element &elem = reg.get<Element>( entity );

    // if (
    //   elem.dimensions_type == Dimensions::FIX_HEIGHT_STRETCH_WIDTH ||
    //   elem.dimensions_type == Dimensions::STRETCH ) {
    //   elem.width = parent_width;
    // }

    elem.pos.x = parent_x;
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
    entt::entity &entity = children[i];
    Element &elem = reg.get<Element>( entity );

    // elem.pos.y = end_of_last + elem.margins.top;
    elem.pos.y = end_of_last;
    // end_of_last = elem.pos.y + elem.dmns.y + elem.margins.bottom;


    // if ( elem.dimensions_type == Dimensions::FIX_WIDTH_STRETCH_HEIGHT ) {
    //   elem.height = parent_height / children.size();
    // }


    end_of_last = elem.pos.y + elem.height;
  }
}


inline void Col_VertSpaceOut(
  entt::registry &reg,
  f32 parent_y,
  f32 parent_height,
  std::vector<entt::entity> children ) {
  f32 total_height = 0;

  for ( u32 i = 0; i < children.size(); i++ ) {
    entt::entity &entity = children[i];
    Element &elem = reg.get<Element>( entity );

    // if (
    //   elem.dimensions_type == Dimensions::FIX_WIDTH_STRETCH_HEIGHT ||
    //   elem.dimensions_type == Dimensions::STRETCH ) {
    //   elem.height = parent_height / children.size();
    // }

    total_height += elem.height;
  }

  f32 total_gap_height = parent_height - total_height;
  u32 num_gaps = children.size() - 1;

  f32 gap_height = total_gap_height / num_gaps;

  f32 end_of_last = parent_y;
  for ( u32 i = 0; i < children.size(); i++ ) {
    entt::entity &entity = children[i];
    Element &elem = reg.get<Element>( entity );

    if ( i % 2 == 0 ) {
      elem.pos.y = end_of_last;
      end_of_last = elem.pos.y + elem.height;
    } else {
      elem.pos.y = end_of_last + gap_height;
      end_of_last = elem.pos.y + elem.height + gap_height;
    }
  }
}

};// namespace UI
