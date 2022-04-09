#pragma once

#include "../common.hpp"
#include <functional>
#include <iterator>
#include <raylib.h>

namespace GUI {

enum class Type {
  PANEL,
  TEXT_LABEL,
  TEXTURE_LABEL,
  TEXT_BUTTON,
  TEXTURE_BUTTON,
};

enum class AlignAxis {
  FLEX_ROW,// Default
  FLEX_COLUMN,
};

enum class AlignSelf {
  AUTO,// Default
  FLEX_START,
  FLEX_END,
  CENTER,
  STRETCH,
};

enum class Alignment {
  FLEX_START,// Default
  FLEX_END,
  CENTER,
  SPACE_BETWEEN,
  SPACE_AROUND,
  SPACE_EVENLY,
  STRETCH
};

inline entt::registry gui_reg;

struct Context {
  entt::entity hot;
  entt::entity active;
};

struct Margins {
  f32 top;
  f32 right;
  f32 bottom;
  f32 left;
};

struct Element {
  Type type;
  bool enabled;
  Color color;
  Vector2 pos;
  Vector2 dmns;

  Margins margins;

  AlignSelf align_self;
};

struct Panel {
  AlignAxis align_axis;
  Alignment align_main;
  Alignment align_cross;
  std::vector<entt::entity> children;
};

struct TextLabel {
  const char *text;
  i32 fontSize;
  Color textColor;
};

struct TextButton {
  TextLabel label;
  std::function<void()> action;
};

struct TextureLabel {
  Texture2D texture;
};

// struct FloatingPanel {
//   Panel panel;
//   Vector2 oldOffset;
// };

inline Rectangle RectangleFromVectors( Vector2, Vector2 );

inline void MainRowAlignChildren(
  Rectangle parent,
  Alignment main_align,
  std::vector<entt::entity> children ) {
  switch ( main_align ) {
    case Alignment::FLEX_START: {
      f32 end_of_last = parent.x;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = gui_reg.get<Element>( entity );

        elem.pos.x = end_of_last + elem.margins.left;
        end_of_last = elem.pos.x + elem.dmns.x + elem.margins.right;
      }
    } break;

    case Alignment::FLEX_END: {
      f32 current_offset = 0;

      for ( std::vector<entt::entity>::reverse_iterator rit = children.rbegin();
            rit != children.rend();
            ++rit ) {
        entt::entity &entity = *rit;
        Element &elem = gui_reg.get<Element>( entity );

        current_offset += ( elem.dmns.x );

        elem.pos.x = parent.width - current_offset;
      }
    } break;

    case Alignment::CENTER: {
      f32 total_width = 0;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = gui_reg.get<Element>( entity );
        total_width += elem.dmns.x;
      }

      f32 x_pos = ( parent.width / 2 ) - ( total_width / 2 );

      f32 end_of_last = x_pos;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = gui_reg.get<Element>( entity );

        elem.pos.x = end_of_last;
        end_of_last = elem.pos.x + elem.dmns.x;
      }
    } break;

    case Alignment::SPACE_BETWEEN: {
      f32 total_width = 0;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = gui_reg.get<Element>( entity );
        total_width += elem.dmns.x;
      }

      f32 total_gap_width = parent.width - total_width;
      u32 num_gaps = children.size() - 1;

      f32 gap_width = total_gap_width / num_gaps;

      f32 end_of_last = parent.x;
      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = gui_reg.get<Element>( entity );

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
  Rectangle parent,
  Alignment cross_align,
  std::vector<entt::entity> children ) {

  for ( u32 i = 0; i < children.size(); i++ ) {
    entt::entity &entity = children[i];
    Element &elem = gui_reg.get<Element>( entity );

    switch ( elem.align_self ) {
      case AlignSelf::AUTO:
        break;
      case AlignSelf::FLEX_START:
        elem.pos.y = parent.y;
        return;
      case AlignSelf::FLEX_END:
        elem.pos.y = parent.height - elem.dmns.y;
        return;
      case AlignSelf::CENTER:
        elem.pos.y = ( parent.height / 2 ) - ( elem.dmns.y / 2 );
        return;
      case AlignSelf::STRETCH:
        elem.pos.y = parent.y;
        elem.dmns.y = parent.height;
        break;
      default:
        // AUTO
        break;
    }

    switch ( cross_align ) {
      case Alignment::FLEX_START:
        elem.pos.y = parent.y;
        break;
      case Alignment::FLEX_END:
        elem.pos.y = parent.height - elem.dmns.y;
        break;
      case Alignment::CENTER:
        elem.pos.y = ( parent.height / 2 ) - ( elem.dmns.y / 2 );
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

inline void MainColumnAlignChildren(
  Rectangle parent,
  Alignment main_align,
  std::vector<entt::entity> children ) {
  switch ( main_align ) {
    case Alignment::FLEX_START: {

      f32 end_of_last = parent.y;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = gui_reg.get<Element>( entity );

        elem.pos.y = end_of_last + elem.margins.top;
        end_of_last = elem.pos.y + elem.dmns.y + elem.margins.bottom;
      }
    } break;

    case Alignment::FLEX_END: {
      f32 current_offset = 0;

      for ( std::vector<entt::entity>::reverse_iterator rit = children.rbegin();
            rit != children.rend();
            ++rit ) {
        entt::entity &entity = *rit;
        Element &elem = gui_reg.get<Element>( entity );

        current_offset += ( elem.dmns.y );

        elem.pos.y = parent.height - current_offset;
      }
    } break;

    case Alignment::CENTER: {
      f32 total_height = 0;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = gui_reg.get<Element>( entity );
        total_height += elem.dmns.y;
      }

      f32 x_pos = ( parent.height / 2 ) - ( total_height / 2 );

      f32 end_of_last = x_pos;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = gui_reg.get<Element>( entity );

        elem.pos.y = end_of_last;
        end_of_last = elem.pos.y + elem.dmns.y;
      }
    } break;

    case Alignment::SPACE_BETWEEN: {
      f32 total_height = 0;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = gui_reg.get<Element>( entity );
        total_height += elem.dmns.y;
      }

      f32 total_gap_height = parent.height - total_height;
      u32 num_gaps = children.size() - 1;

      f32 gap_height = total_gap_height / num_gaps;

      f32 end_of_last = parent.y;
      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = gui_reg.get<Element>( entity );

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
  Rectangle parent,
  Alignment cross_align,
  std::vector<entt::entity> children ) {

  for ( u32 i = 0; i < children.size(); i++ ) {
    entt::entity &entity = children[i];
    Element &elem = gui_reg.get<Element>( entity );

    switch ( elem.align_self ) {
      case AlignSelf::AUTO:
        break;
      case AlignSelf::FLEX_START:
        elem.pos.x = parent.x;
        return;
      case AlignSelf::FLEX_END:
        elem.pos.x = parent.width - elem.dmns.x;
        return;
      case AlignSelf::CENTER:
        elem.pos.x = ( parent.width / 2 ) - ( elem.dmns.x / 2 );
        return;
      case AlignSelf::STRETCH:
        elem.pos.x = parent.x;
        elem.dmns.x = parent.width;
        break;
      default:
        // AUTO
        break;
    }

    switch ( cross_align ) {
      case Alignment::FLEX_START:
        elem.pos.x = parent.x;
        break;
      case Alignment::FLEX_END:
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

inline void Layout( Panel root, f32 screen_width, f32 screen_height ) {

  if ( root.align_axis == AlignAxis::FLEX_ROW ) {
    MainRowAlignChildren(
      { 0, 0, screen_width, screen_height },
      root.align_main,
      root.children );

    CrossRowAlignChildren(
      { 0, 0, screen_width, screen_height },
      root.align_cross,
      root.children );
  } else {
    MainColumnAlignChildren(
      { 0, 0, screen_width, screen_height },
      root.align_main,
      root.children );

    CrossColumnAlignChildren(
      { 0, 0, screen_width, screen_height },
      root.align_cross,
      root.children );
  }

  for ( auto &panel_entity: GUI::gui_reg.view<GUI::Panel>() ) {
    GUI::Panel &panel = GUI::gui_reg.get<GUI::Panel>( panel_entity );
    GUI::Element &panel_elem = GUI::gui_reg.get<GUI::Element>( panel_entity );
    if ( panel.align_axis == AlignAxis::FLEX_ROW ) {
      MainRowAlignChildren(
        RectangleFromVectors( panel_elem.pos, panel_elem.dmns ),
        panel.align_main,
        panel.children );

      CrossRowAlignChildren(
        RectangleFromVectors( panel_elem.pos, panel_elem.dmns ),
        panel.align_cross,
        panel.children );
    } else {

      MainColumnAlignChildren(
        RectangleFromVectors( panel_elem.pos, panel_elem.dmns ),
        panel.align_main,
        panel.children );

      CrossColumnAlignChildren(
        RectangleFromVectors( panel_elem.pos, panel_elem.dmns ),
        panel.align_cross,
        panel.children );
    }
  }
}

inline bool DoItem(
  Context &context,
  entt::entity entity,
  bool inside,
  bool mouseWentUp,
  bool mouseWentDown ) {
  bool result = false;

  if ( entity == context.active ) {
    if ( mouseWentUp ) {
      if ( entity == context.hot )
        result = true;// do the button action

      context.active = entt::null;
    }
  } else if ( entity == context.hot ) {
    if ( mouseWentDown )
      context.active = entity;
  }

  if ( inside ) {
    if ( context.active == entt::null )
      context.hot = entity;
  }

  return result;
}

inline Rectangle RectangleFromVectors( Vector2 pos, Vector2 dimensions ) {
  return { pos.x, pos.y, dimensions.x, dimensions.y };
}

inline bool MouseWasOverUI() {
  bool inside = false;

  for ( auto &entity: GUI::gui_reg.view<GUI::Element>() ) {
    GUI::Element elem = GUI::gui_reg.get<GUI::Element>( entity );

    inside = CheckCollisionPointRec(
      GetMousePosition(),
      GUI::RectangleFromVectors( elem.pos, elem.dmns ) );

    if ( inside )
      return true;
  }

  return inside;
}


// inline bool DoFloatingPanel(
//   Context &context,
//   Panel &panel,
//   bool inside,
//   bool mouseWentUp,
//   bool mouseWentDown,
//   bool mouseHeldDown ) {
//   bool result = false;

//   if ( panel.elem == context.active ) {
//     if ( !mouseWentUp && mouseHeldDown )
//       result = true;
//   } else if ( panel.elem == context.hot ) {
//     if ( mouseWentDown )
//       context.active = panel.elem.entity;
//   }

//   if ( inside ) {
//     if ( context.active == entt::null )
//       context.hot = panel.elem.entity;
//   }

//   DrawRectangleV( panel.elem.pos, panel.elem.dmns, panel.elem.color );

//   return result;
// }

};// namespace GUI
