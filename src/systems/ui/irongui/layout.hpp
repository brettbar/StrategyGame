#include "../../../common.hpp"

#include "defns.hpp"

namespace IRONGUI {

inline void Layout( entt::registry &, Panel, f32, f32 );
inline void MainRowAlignChildren(
  entt::registry &,
  Rectangle,
  Alignment,
  std::vector<entt::entity> );
inline void CrossRowAlignChildren(
  entt::registry &,
  Rectangle,
  Alignment,
  std::vector<entt::entity> );
inline void MainColumnAlignChildren(
  entt::registry &,
  Rectangle,
  Alignment,
  std::vector<entt::entity> );
inline void CrossColumnAlignChildren(
  entt::registry &,
  Rectangle,
  Alignment,
  std::vector<entt::entity> );
inline Rectangle RectangleFromVectors( Vector2, Vector2 );

inline void
Layout( entt::registry &reg, Panel root, f32 screen_width, f32 screen_height ) {
  if ( root.children_align_axis == AlignAxis::ROW ) {
    MainRowAlignChildren(
      reg,
      { 0, 0, screen_width, screen_height },
      root.children_main_align,
      root.children );

    CrossRowAlignChildren(
      reg,
      { 0, 0, screen_width, screen_height },
      root.children_cross_align,
      root.children );
  } else {
    MainColumnAlignChildren(
      reg,
      { 0, 0, screen_width, screen_height },
      root.children_main_align,
      root.children );

    CrossColumnAlignChildren(
      reg,
      { 0, 0, screen_width, screen_height },
      root.children_cross_align,
      root.children );
  }

  for ( auto &panel_entity: reg.view<Panel>() ) {
    Panel &panel = reg.get<Panel>( panel_entity );
    Element &panel_elem = reg.get<Element>( panel_entity );
    if ( panel.children_align_axis == AlignAxis::ROW ) {
      MainRowAlignChildren(
        reg,
        RectangleFromVectors( panel_elem.pos, panel_elem.dmns ),
        panel.children_main_align,
        panel.children );

      CrossRowAlignChildren(
        reg,
        RectangleFromVectors( panel_elem.pos, panel_elem.dmns ),
        panel.children_cross_align,
        panel.children );
    } else {

      MainColumnAlignChildren(
        reg,
        RectangleFromVectors( panel_elem.pos, panel_elem.dmns ),
        panel.children_main_align,
        panel.children );

      CrossColumnAlignChildren(
        reg,
        RectangleFromVectors( panel_elem.pos, panel_elem.dmns ),
        panel.children_cross_align,
        panel.children );
    }
  }
}


inline void MainRowAlignChildren(
  entt::registry &reg,
  Rectangle parent,
  Alignment main_align,
  std::vector<entt::entity> children ) {
  switch ( main_align ) {
    case Alignment::START: {
      f32 end_of_last = parent.x;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = reg.get<Element>( entity );


        // if ( elem.type == IRONGUI::Type::PANEL ) {
        //   if ( reg.get<Panel>( entity ).pos_absolute ) {
        //     continue;
        //   }
        // }

        elem.pos.x = end_of_last + elem.margins.left;
        end_of_last = elem.pos.x + elem.dmns.x + elem.margins.right;
      }
    } break;

    case Alignment::END: {
      f32 current_offset = 0;

      for ( std::vector<entt::entity>::reverse_iterator rit = children.rbegin();
            rit != children.rend();
            ++rit ) {
        entt::entity &entity = *rit;
        Element &elem = reg.get<Element>( entity );

        current_offset += ( elem.dmns.x );

        elem.pos.x = parent.width - current_offset;
      }
    } break;

    case Alignment::CENTER: {
      f32 total_width = 0;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = reg.get<Element>( entity );
        total_width += elem.dmns.x;
      }

      f32 x_pos = ( parent.width / 2 ) - ( total_width / 2 );

      f32 end_of_last = x_pos;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = reg.get<Element>( entity );

        elem.pos.x = end_of_last;
        end_of_last = elem.pos.x + elem.dmns.x;
      }
    } break;

    case Alignment::SPACE_BETWEEN: {
      f32 total_width = 0;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = reg.get<Element>( entity );
        total_width += elem.dmns.x;
      }

      f32 total_gap_width = parent.width - total_width;
      u32 num_gaps = children.size() - 1;

      f32 gap_width = total_gap_width / num_gaps;

      f32 end_of_last = parent.x;
      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = reg.get<Element>( entity );

        if ( i % 2 == 0 ) {
          elem.pos.x = end_of_last;
          end_of_last = elem.pos.x + elem.dmns.x;
        } else {
          elem.pos.x = end_of_last + gap_width;
          end_of_last = elem.pos.x + elem.dmns.x + gap_width;
        }
      }

    } break;
    // case Alignment::SPACE_AROUND:
    //   break;
    // case Alignment::SPACE_EVENLY: {
    // } break;
    // case Alignment::STRETCH:
    //   break;
    default:
      // Flex Start
      break;
  }
}

inline void CrossRowAlignChildren(
  entt::registry &reg,
  Rectangle parent,
  Alignment cross_align,
  std::vector<entt::entity> children ) {

  for ( u32 i = 0; i < children.size(); i++ ) {
    entt::entity &entity = children[i];
    Element &elem = reg.get<Element>( entity );

    if ( elem.align_self != SelfAlign::AUTO ) {

      switch ( elem.align_self ) {
        case SelfAlign::START:
          elem.pos.y = parent.y;
          return;
        case SelfAlign::END:
          elem.pos.y = parent.height - elem.dmns.y;
          return;
        case SelfAlign::CENTER:
          elem.pos.y = ( parent.height / 2 ) - ( elem.dmns.y / 2 );
          return;
        case SelfAlign::STRETCH:
          elem.pos.y = parent.y;
          elem.dmns.y = parent.height;
          break;
        default:
          break;
      }
    } else {
      switch ( cross_align ) {
        case Alignment::START:
          elem.pos.y = parent.y;
          break;
        case Alignment::END:
          elem.pos.y = parent.height - elem.dmns.y;
          break;
        case Alignment::CENTER:
          elem.pos.y = ( parent.height / 2 ) - ( elem.dmns.y / 2 );
          break;
        case Alignment::SPACE_BETWEEN:
          break;
        case Alignment::SPACE_AROUND:
          break;
          break;
        case Alignment::STRETCH:
          break;
        default:
          // Flex Start
          break;
      }
    }
  }
}

inline void MainColumnAlignChildren(
  entt::registry &reg,
  Rectangle parent,
  Alignment main_align,
  std::vector<entt::entity> children ) {
  switch ( main_align ) {
    case Alignment::START: {

      f32 end_of_last = parent.y;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = reg.get<Element>( entity );

        elem.pos.y = end_of_last + elem.margins.top;
        end_of_last = elem.pos.y + elem.dmns.y + elem.margins.bottom;
      }
    } break;

    case Alignment::END: {
      f32 current_offset = 0;

      for ( std::vector<entt::entity>::reverse_iterator rit = children.rbegin();
            rit != children.rend();
            ++rit ) {
        entt::entity &entity = *rit;
        Element &elem = reg.get<Element>( entity );

        current_offset += ( elem.dmns.y );

        elem.pos.y = parent.height - current_offset;
      }
    } break;

    case Alignment::CENTER: {
      f32 total_height = 0;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = reg.get<Element>( entity );
        total_height += elem.dmns.y;
      }

      f32 x_pos = ( parent.height / 2 ) - ( total_height / 2 );

      f32 end_of_last = x_pos;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = reg.get<Element>( entity );

        elem.pos.y = end_of_last + elem.margins.top;
        end_of_last = elem.pos.y + elem.dmns.y;
      }
    } break;

    case Alignment::SPACE_BETWEEN: {
      f32 total_height = 0;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = reg.get<Element>( entity );
        total_height += elem.dmns.y;
      }

      f32 total_gap_height = parent.height - total_height;
      u32 num_gaps = children.size() - 1;

      f32 gap_height = total_gap_height / num_gaps;

      f32 end_of_last = parent.y;
      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = reg.get<Element>( entity );

        if ( i % 2 == 0 ) {
          elem.pos.y = end_of_last;
          end_of_last = elem.pos.y + elem.dmns.y;
        } else {
          elem.pos.y = end_of_last + gap_height;
          end_of_last = elem.pos.y + elem.dmns.y + gap_height;
        }
      }

    } break;
    // case Alignment::SPACE_AROUND:
    //   break;
    // case Alignment::SPACE_EVENLY: {
    // } break;
    // case Alignment::STRETCH:
    //   break;
    default:
      // Flex Start
      break;
  }
}

inline void CrossColumnAlignChildren(
  entt::registry &reg,
  Rectangle parent,
  Alignment cross_align,
  std::vector<entt::entity> children ) {

  for ( u32 i = 0; i < children.size(); i++ ) {
    entt::entity &entity = children[i];
    Element &elem = reg.get<Element>( entity );

    switch ( elem.align_self ) {
      case SelfAlign::AUTO:
        break;
      case SelfAlign::START:
        elem.pos.x = parent.x;
        return;
      case SelfAlign::END:
        elem.pos.x = parent.width - elem.dmns.x;
        return;
      case SelfAlign::CENTER:
        elem.pos.x = ( parent.width / 2 ) - ( elem.dmns.x / 2 );
        return;
      case SelfAlign::STRETCH:
        elem.pos.x = parent.x;
        elem.dmns.x = parent.width;
        break;
      default:
        // AUTO
        break;
    }

    switch ( cross_align ) {
      case Alignment::START:
        elem.pos.x = parent.x;
        break;
      case Alignment::END:
        elem.pos.x = parent.width - elem.dmns.x;
        break;
      case Alignment::CENTER:
        elem.pos.x = ( parent.width / 2 ) - ( elem.dmns.x / 2 );
        break;
      case Alignment::SPACE_BETWEEN:
        break;
      case Alignment::SPACE_AROUND:
        break;
      case Alignment::SPACE_EVENLY:
        break;
      case Alignment::STRETCH:
        break;
      default:
        // Flex Start
        break;
    }
  }
}

};// namespace IRONGUI
