#pragma once


#include "../components/actor.hpp"
#include "../components/province.hpp"
#include "../components/selected.hpp"
#include "../renderer/fonts.hpp"
#include "../renderer/textures.hpp"
#include "column_alignment.hpp"
#include "row_alignment.hpp"
#include "ui_components.hpp"
#include "widgets/context_panel.hpp"
#include <raylib.h>
#include <raymath.h>

namespace UI {

inline entt::registry reg;
inline Context context;

inline void Init( TextureCache & );
inline void Draw();
inline void LayoutPanel( Panel &, entt::entity );
inline bool DoInteraction( entt::entity, bool, bool, bool );
inline void DrawElement( TextureCache &, FontCache &, entt::entity, Element & );

inline void SelectListener( entt::registry &, entt::entity );
inline void DeSelectListener();

inline void SelectListener( entt::registry &reg, entt::entity entity ) {
  if ( reg.all_of<Province::Component>( entity ) ) {

    auto stats = ui_lookup.at( "context_panel" );
    Element &elem = reg.get<Element>( stats );

    elem.enabled = true;

  } else if ( reg.all_of<Actor::Component>( entity ) ) {
    auto actor = reg.get<Actor::Component>( entity );
    printf( "Actor: %s \n", actor.name );

    entt::entity context_label = ui_lookup.at( "context_label" );
    reg.get<TextLabel>( context_label ).text = actor.name;
  }
}

inline void DeSelectListener() {
  auto stats = ui_lookup.at( "settlement_stats" );
  Element &elem = reg.get<Element>( stats );
  elem.enabled = false;
}


inline void Init( TextureCache &texture_cache ) {
  reg.on_construct<Selected::Component>().connect<&SelectListener>();
  reg.on_destroy<Selected::Component>().connect<&DeSelectListener>();

  CreateContextPanel( reg, texture_cache );
}

inline void Draw( TextureCache &texture_cache, FontCache &font_cache ) {
  vec2 mousePos = GetMousePosition();
  bool mouseWentUp = IsMouseButtonReleased( 0 );
  bool mouseWentDown = IsMouseButtonPressed( 0 );
  bool mouseHeldDown = IsMouseButtonDown( 0 );
  bool overAnyElem = false;
  const f32 screen_width = GetScreenWidth();
  const f32 screen_height = GetScreenHeight();

  auto ui_elements = reg.view<Element>();
  auto base_panels = reg.view<BasePanel>();

  if ( screen_width > 1920 ) {
    SCALE = 2.0;
  } else if ( screen_width > 2560 ) {
    SCALE = 3.0;
  } else if ( screen_width >= 3840 ) {
    SCALE = 4.0;
  }

  for ( auto &ent: base_panels ) {
    Element &elem = reg.get<Element>( ent );
    BasePanel &parent = reg.get<BasePanel>( ent );

    if ( !elem.enabled )
      continue;

    // 0. Resize the parent panel.
    parent.update( parent );

    LayoutPanel( parent, ent );

    for ( auto &child: parent.children ) {
      assert( reg.any_of<Panel>( child ) );
      Element &elem = reg.get<Element>( child );
      Panel &panel = reg.get<Panel>( child );
      if ( !elem.enabled )
        continue;

      LayoutPanel( panel, child );
    }
  }

  // Check for interactions and Draw everything
  for ( auto &entity: ui_elements ) {
    Element &elem = reg.get<Element>( entity );

    if ( !elem.enabled )
      continue;

    rect &transform = GetTransform( reg, entity );
    bool inside = CheckCollisionPointRec( GetMousePosition(), transform );

    if ( !overAnyElem )
      overAnyElem = inside;


    // Check for interactions
    if ( DoInteraction( entity, inside, mouseWentUp, mouseWentDown ) ) {
    }

    // DRAW
    DrawElement( texture_cache, font_cache, entity, elem );
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

inline void LayoutPanel( Panel &panel, entt::entity entity ) {
  f32 total_height = 0;
  f32 total_width = 0;
  f32 tallest_child = 0;
  f32 widest_child = 0;
  f32 end_of_last_x = panel.transform.x;
  f32 end_of_last_y = panel.transform.y;

  for ( auto &child: panel.children ) {
    Rectangle &elem = GetTransform( reg, child );

    total_width += elem.width;
    total_height += elem.height;

    if ( elem.width > widest_child )
      widest_child = elem.width;

    if ( elem.height > tallest_child )
      tallest_child = elem.height;
  }


  if ( !reg.any_of<BasePanel>( entity ) ) {
    if ( panel.children_axis == Axis::ROW ) {
      panel.transform.width = total_width;
      panel.transform.height = tallest_child;
    } else if ( panel.children_axis == Axis::COLUMN ) {
      panel.transform.width = widest_child;
      panel.transform.height = total_height;
    }
  }

  // For each child
  for ( auto &child: panel.children ) {
    Rectangle &transform = GetTransform( reg, child );

    // 1. Resize the child to its proper scale.
    switch ( reg.get<Element>( child ).type ) {
      case Type::PANEL: {
        LayoutPanel( reg.get<Panel>( child ), child );
      } break;
      case Type::TEXT_BUTTON: {
      } break;
      case Type::TEXTURE_BUTTON: {
        Texture2D texture = reg.get<TextureButton>( child ).texture;
        transform.width = texture.width * UI::SCALE;
        transform.height = texture.height * UI::SCALE;
      } break;
    }

    if ( panel.children_axis == Axis::ROW ) {
      // 2. Set the child x position based on alignment style.
      switch ( panel.children_horiz_align ) {
        case Align::START: {
          transform.x = end_of_last_x;
          end_of_last_x = transform.x + transform.width;
        } break;
        case Align::SPACE_OUT: {
        } break;
      }

      // 3. Set the child y position based on alignment style.
      switch ( panel.children_vert_align ) {
        case Align::START: {
          transform.y = panel.transform.y;
        } break;
      }


    } else if ( panel.children_axis == Axis::COLUMN ) {
      // 2. Set the child x position based on alignment style.
      switch ( panel.children_horiz_align ) {
        case Align::START: {
          transform.x = panel.transform.x;
        } break;
        case Align::SPACE_OUT: {
        } break;
      }

      // 3. Set the child y position based on alignment style.
      switch ( panel.children_vert_align ) {
        case Align::START: {
          transform.y = end_of_last_y;
          end_of_last_y = transform.y + transform.height;
        } break;
      }
    }
  }
}

inline bool DoInteraction(
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

inline void DrawElement(
  TextureCache &texture_cache,
  FontCache &font_cache,
  entt::entity entity,
  Element &elem ) {

  switch ( elem.type ) {
    case Type::BASE_PANEL: {
      auto &panel = reg.get<BasePanel>( entity );
      DrawRectangleV(
        { panel.transform.x, panel.transform.y },
        { panel.transform.width, panel.transform.height },
        panel.background );
    } break;
    case Type::PANEL: {
      auto &panel = reg.get<Panel>( entity );
      DrawRectangleV(
        { panel.transform.x, panel.transform.y },
        { panel.transform.width, panel.transform.height },
        panel.background );
    } break;
    case Type::TEXTURE_BUTTON: {
      auto &button = reg.get<TextureButton>( entity );
      DrawTextureEx(
        button.texture,
        { button.transform.x, button.transform.y },
        0.0,
        SCALE,
        WHITE );
    } break;

    case Type::TEXT_BUTTON: {
      auto &button = reg.get<TextButton>( entity );
      DrawRectangleV(
        { button.transform.x, button.transform.y },
        { button.transform.width, button.transform.height },
        button.background );

      DrawTextEx(
        font_cache[hstr{ "font_romulus" }]->font,
        button.text.c_str(),
        {
          button.transform.x,
          button.transform.y + ( 0.5f * button.transform.height ),
        },
        button.font_size,
        2.0,
        button.text_color );
    } break;
  }
}


inline bool MouseIsOverUI() {
  // This is almost sufficent, but we need to account for panels too
  // not just items that can be active
  return context.active != entt::null || context.hot != entt::null;
}
};// namespace UI
