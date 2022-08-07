#pragma once

#include "../../common.hpp"
#include "../../renderer/fonts.hpp"
#include <functional>
#include <iterator>
#include <raylib.h>

namespace UI {

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

inline entt::registry reg;

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
  // const char *text;
  std::string text;
  i32 font_size;
  Color text_color;
  // Alignment text_align;
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

inline void Layout( Panel, f32, f32 );
inline Rectangle RectangleFromVectors( Vector2, Vector2 );

inline Context context = {
  .hot = entt::null,
  .active = entt::null,
};

template<typename Flag>
inline void Draw(
  Panel &curr_content,
  entt::basic_view<entt::entity, entt::get_t<Element, Flag>, entt::exclude_t<>>
    all_view,
  entt::
    basic_view<entt::entity, entt::get_t<Element, Flag>, entt::exclude_t<Panel>>
      items_view,
  FontCache &font_cache ) {
  Vector2 mousePos = GetMousePosition();
  bool mouseWentUp = IsMouseButtonReleased( 0 );
  bool mouseWentDown = IsMouseButtonPressed( 0 );
  bool mouseHeldDown = IsMouseButtonDown( 0 );
  bool overAnyElem = false;

  // 0. Get screen dims for current frame
  const f32 screen_width = GetScreenWidth();
  const f32 screen_height = GetScreenHeight();

  // auto all_view = reg.view<Element>();
  // auto items_view = reg.view<Element>( entt::exclude<Panel> );

  auto font = font_cache[hstr{ "font_romulus" }]->font;

  Layout( curr_content, screen_width, screen_height );

  for ( auto &entity: items_view ) {
    Element &elem = reg.get<Element>( entity );

    if ( !elem.enabled )
      continue;

    bool inside = CheckCollisionPointRec(
      GetMousePosition(),
      RectangleFromVectors( { elem.pos.x, elem.pos.y }, elem.dmns ) );

    if ( !overAnyElem )
      overAnyElem = inside;


    if ( DoItem( context, entity, inside, mouseWentUp, mouseWentDown ) ) {
      switch ( elem.type ) {
        case Type::TEXT_BUTTON: {
          auto &button = reg.get<TextButton>( entity );
          button.action();
        } break;
        case Type::TEXTURE_BUTTON: {
        } break;
        default:
          break;
      }
    }
  }


  // 6. Draw Everything
  for ( auto &entity: all_view ) {
    Element &elem = reg.get<Element>( entity );
    if ( !elem.enabled )
      continue;

    switch ( elem.type ) {
      case Type::PANEL: {
        // auto &panel = gui_reg.get<Panel>( entity );

        DrawRectangleV( elem.pos, elem.dmns, elem.color );

      } break;

      case Type::TEXT_LABEL: {
        auto &label = reg.get<TextLabel>( entity );
        DrawRectangleV( elem.pos, elem.dmns, elem.color );


        DrawTextEx(
          // font_cache.handle( hstr{ "font_romulus" } )->font,
          font,
          label.text.c_str(),
          {
            elem.pos.x,
            elem.pos.y + ( 0.25f * elem.dmns.y ),
          },
          label.font_size,
          2.0f,
          label.text_color );

        // DrawText(
        //   label.text.c_str(),
        //   elem.pos.x,
        //   elem.pos.y + ( 0.25 * elem.dmns.y ),
        //   label.font_size,
        //   label.text_color );
      } break;

      case Type::TEXT_BUTTON: {
        auto &button = reg.get<TextButton>( entity );
        DrawRectangleV( elem.pos, elem.dmns, elem.color );

        DrawTextEx(
          font,
          button.label.text.c_str(),
          {
            elem.pos.x,
            elem.pos.y + ( 0.5f * elem.dmns.y ),
          },
          button.label.font_size,
          2.0f,
          button.label.text_color );

      } break;

      case Type::TEXTURE_LABEL: {
        auto &label = reg.get<TextureLabel>( entity );
        DrawTexture( label.texture, elem.pos.x, elem.pos.y, elem.color );
      } break;

      default:
        break;
    }
  }

  std::string hot_str = std::to_string( (std::uint32_t) context.hot );
  std::string active_str = std::to_string( (std::uint32_t) context.active );

  if ( !overAnyElem ) {
    context.hot = entt::null;
    context.active = entt::null;
  }

  if ( context.hot == entt::null ) {
    hot_str = "-1";
  }

  if ( context.active == entt::null ) {
    active_str = "-1";
  }

  DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
  DrawFPS( GetScreenWidth() - 100, 2 );

  // TODO make this a part of debug
  DrawRectangle( GetScreenWidth() - 200, 102, 200, 24.0f, BLACK );
  std::string foo = "hot: " + hot_str;
  DrawText( foo.c_str(), GetScreenWidth() - 200, 102, 24.0f, RED );

  DrawRectangle( GetScreenWidth() - 200, 152, 200, 24.0f, BLACK );
  std::string bar = "active: " + active_str;
  DrawText( bar.c_str(), GetScreenWidth() - 200, 152, 24.0f, RED );
}


inline void MainRowAlignChildren(
  Rectangle parent,
  Alignment main_align,
  std::vector<entt::entity> children ) {
  switch ( main_align ) {
    case Alignment::FLEX_START: {
      f32 end_of_last = parent.x;

      for ( u32 i = 0; i < children.size(); i++ ) {
        entt::entity &entity = children[i];
        Element &elem = reg.get<Element>( entity );

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
  Rectangle parent,
  Alignment cross_align,
  std::vector<entt::entity> children ) {

  for ( u32 i = 0; i < children.size(); i++ ) {
    entt::entity &entity = children[i];
    Element &elem = reg.get<Element>( entity );

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
        Element &elem = reg.get<Element>( entity );

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
  Rectangle parent,
  Alignment cross_align,
  std::vector<entt::entity> children ) {

  for ( u32 i = 0; i < children.size(); i++ ) {
    entt::entity &entity = children[i];
    Element &elem = reg.get<Element>( entity );

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

  for ( auto &panel_entity: reg.view<Panel>() ) {
    Panel &panel = reg.get<Panel>( panel_entity );
    Element &panel_elem = reg.get<Element>( panel_entity );
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

  for ( auto &entity: reg.view<Element>() ) {
    Element elem = reg.get<Element>( entity );

    inside = CheckCollisionPointRec(
      GetMousePosition(),
      RectangleFromVectors( elem.pos, elem.dmns ) );

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

};// namespace UI
