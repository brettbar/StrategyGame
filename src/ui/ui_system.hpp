#pragma once


#include "../renderer/fonts.hpp"
#include "../renderer/textures.hpp"
#include "column_alignment.hpp"
#include "row_alignment.hpp"
#include "widgets/context_panel.hpp"
#include <raylib.h>
#include <raymath.h>

namespace UI {

inline entt::registry reg;
inline Context context;

inline void Init();
inline void Draw();
inline bool DoItem( entt::entity, bool, bool, bool );


inline void Init() {
  //
  CreateContextPanel( reg );
}// namespace UI

inline void Draw( TextureCache &texture_cache, FontCache &font_cache ) {
  vec2 mousePos = GetMousePosition();
  bool mouseWentUp = IsMouseButtonReleased( 0 );
  bool mouseWentDown = IsMouseButtonPressed( 0 );
  bool mouseHeldDown = IsMouseButtonDown( 0 );
  bool overAnyElem = false;
  const f32 screen_width = GetScreenWidth();
  const f32 screen_height = GetScreenHeight();

  auto ui_elements = reg.view<Element>();
  auto panels = reg.view<Panel>();


  // LAYOUT
  for ( auto &ent: panels ) {
    Element &elem = reg.get<Element>( ent );
    Panel &panel = reg.get<Panel>( ent );

    switch ( elem.type ) {
      case Type::PANEL:

        if ( panel.self_positioned )
          panel.Update( elem );

        if ( panel.children_axis == Axis::ROW )
          Row_Align( reg, panel, elem );
        else
          Col_Align( reg, panel, elem );

        break;
    }
  }


  // Check for interactions and Draw everything
  for ( auto &entity: ui_elements ) {
    Element &elem = reg.get<Element>( entity );

    bool inside = CheckCollisionPointRec(
      GetMousePosition(),
      { elem.pos.x, elem.pos.y, elem.width, elem.height } );

    if ( !overAnyElem )
      overAnyElem = inside;


    // Check for interactions
    if ( DoItem( entity, inside, mouseWentUp, mouseWentDown ) ) {
    }

    // DRAW
    switch ( elem.type ) {
      case Type::PANEL: {
        DrawRectangleV(
          elem.pos,
          { elem.width, elem.height },
          elem.background );
      } break;
      case Type::TEXTURE_BUTTON: {
        auto &button = reg.get<TextureButton>( entity );
        DrawTextureEx(
          texture_cache[button.texture_key]->texture,
          elem.pos,
          0.0,
          1.0,
          elem.background );
      } break;

      case Type::TEXT_BUTTON: {
        auto &button = reg.get<TextButton>( entity );
        DrawRectangleV(
          elem.pos,
          { elem.width, elem.height },
          elem.background );

        DrawTextEx(
          font_cache[hstr{ "font_romulus" }]->font,
          button.text.c_str(),
          {
            elem.pos.x,
            elem.pos.y + ( 0.5f * elem.height ),
          },
          button.font_size,
          2.0,
          button.text_color );
      } break;
    }
  }


  std::string hot_str = std::to_string( (std::uint32_t) context.hot );
  std::string active_str = std::to_string( (std::uint32_t) context.active );

  if ( !overAnyElem ) {
    context.hot = entt::null;
    context.active = entt::null;
  }

  if ( context.hot == entt::null )
    hot_str = "-1";

  if ( context.active == entt::null )
    active_str = "-1";

  DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
  DrawFPS( GetScreenWidth() - 100, 2 );

  // TODO make this a part of debug
  DrawRectangle( GetScreenWidth() - 200, 102, 200, 24.0f, BLACK );
  std::string foo = "hot: " + hot_str;
  DrawText( foo.c_str(), GetScreenWidth() - 200, 102, 24.0f, RED );

  DrawRectangle( GetScreenWidth() - 200, 152, 200, 24.0f, BLACK );
  std::string bar = "active: " + active_str;
  DrawText( bar.c_str(), GetScreenWidth() - 200, 152, 24.0f, RED );


  // DrawTexturePro(texture_cache[hstr{"context_panel"}]->texture, {747, 448}, {}, {0, 0}, 0, WHITE);
}

inline bool DoItem(
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
  // This is almost sufficent, but we need to account for panels too
  // not just items that can be active
  return context.active != entt::null || context.hot != entt::null;
}

};// namespace UI
