#pragma once

#include "../../../common.hpp"
#include "../../../renderer/fonts.hpp"

#include "layout.hpp"

#include <functional>
#include <iterator>

namespace IRONGUI {

inline entt::registry reg;

// @TODO - We probably want to make Panels able to be active/hot instead
// of just items
struct Context {
  entt::entity hot;
  entt::entity active;
};


// struct FloatingPanel {
//   Panel panel;
//   Vector2 oldOffset;
// };

template<typename Flag>
inline void Draw(
  Panel &curr_content,
  entt::basic_view<entt::entity, entt::get_t<Element, Flag>, entt::exclude_t<>>,
  entt::basic_view<
    entt::entity,
    entt::get_t<Element, Flag>,
    entt::exclude_t<Panel>>,
  FontCache & );
inline bool DoItem( Context &context, entt::entity, bool, bool, bool );
inline bool MouseIsOverUI();

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

  Layout( reg, curr_content, screen_width, screen_height );

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

inline bool MouseIsOverUI() {
  // bool inside = false;

  // for ( auto &entity: reg.view<Element>() ) {
  //   Element elem = reg.get<Element>( entity );

  //   inside = CheckCollisionPointRec(
  //     GetMousePosition(),
  //     RectangleFromVectors( elem.pos, elem.dmns ) );

  //   if ( inside )
  //     return true;
  // }

  // return inside;

  // This is almost sufficent, but we need to account for panels too
  // not just items that can be active
  return context.active != entt::null || context.hot != entt::null;
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


};// namespace IRONGUI
