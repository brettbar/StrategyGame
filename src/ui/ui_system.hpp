#pragma once


#include "../../.old/ui/column_alignment.hpp"
#include "../../.old/ui/row_alignment.hpp"
#include "../components/actor.hpp"
#include "../components/event.hpp"
#include "../components/selected.hpp"
#include "../components/settlement.hpp"
#include "../global.hpp"
#include "../renderer/fonts.hpp"
#include "../renderer/textures.hpp"
#include "../systems/event_system.hpp"
#include "../systems/selection_system.hpp"
#include "ui_components.hpp"
#include "widgets/actor_context_panel.hpp"
#include "widgets/overview_banner.hpp"
#include "widgets/settlement_context_panel.hpp"

#include <iostream>
#include <raylib.h>
#include <raymath.h>

namespace UI {
inline Context context;

inline void Init( entt::registry &, TextureCache & );
inline void Update();
inline void Draw();
inline void LayoutPanel( Panel &, entt::entity );
inline void ResizeElement( entt::entity, Element & );
inline bool DoInteraction( entt::entity, bool, bool, bool, bool );
inline void DrawElement( TextureCache &, entt::entity, Element & );
inline void SelectListener( entt::registry &, entt::entity );
inline void DeSelectListener();
inline void ToggleElement( entt::entity, bool );

inline std::string ActiveId();
inline std::string HotId();

inline void Init( entt::registry &game_reg, TextureCache &texture_cache ) {

  OverviewBanner();
  SettlementContextPanel( texture_cache );
  ActorContextPanel( texture_cache );

  game_reg.on_construct<Selected::Component>().connect<&SelectListener>();
  game_reg.on_destroy<Selected::Component>().connect<&DeSelectListener>();
}

inline void Update( entt::registry &game_reg ) {
  // if ( SelectionSystem::selected_entity == entt::null )
  //   return;

  // if ( game_reg.all_of<Province::Component>(
  //        SelectionSystem::selected_entity ) ) {

  //   entt::entity settlement_context = ui_lookup["settlement_context"];
  //   Element &elem = ui_reg.get<Element>( settlement_context );
  //   if ( !elem.enabled )
  //     return;

  //   // Settlement::Component &settlement =
  //   //   game_reg.get<Settlement::Component>( SelectionSystem::selected_entity );

  //   // ui_reg.get<TextLabel>( ui_lookup["province_name"] ).text = settlement.name;
  //   // ui_reg.get<TextLabel>( ui_lookup["province_population"] ).text =
  //   //   std::to_string( settlement.population.current );
  //   // ui_reg.get<TextLabel>( ui_lookup["province_development"] ).text =
  //   //   Settlement::development[settlement.development];
  // }
}

inline void Draw( TextureCache &texture_cache ) {
  vec2 mousePos = GetMousePosition();
  bool mouseWentUp = IsMouseButtonReleased( 0 );
  bool mouseWentDown = IsMouseButtonPressed( 0 );
  bool mouseHeldDown = IsMouseButtonDown( 0 );
  bool over_any_elem = false;
  const f32 screen_width = GetScreenWidth();
  const f32 screen_height = GetScreenHeight();

  auto ui_elements = ui_reg.view<Element>();
  auto base_panels = ui_reg.view<BasePanel>();

  if ( screen_width > 1920 ) {
    SCALE = 2.0;
  } else if ( screen_width > 2560 ) {
    SCALE = 3.0;
  } else if ( screen_width >= 3840 ) {
    SCALE = 4.0;
  }

  for ( auto &ent: base_panels ) {
    Element &elem = ui_reg.get<Element>( ent );
    BasePanel &parent = ui_reg.get<BasePanel>( ent );

    if ( !elem.enabled )
      continue;

    // 0. Resize the parent panel.
    parent.update( elem, parent );

    LayoutPanel( parent, ent );

    for ( auto &child: parent.children ) {
      assert( ui_reg.all_of<Panel>( child ) );

      Element &elem = ui_reg.get<Element>( child );
      Panel &panel = ui_reg.get<Panel>( child );
      if ( !elem.enabled )
        continue;

      LayoutPanel( panel, child );
    }
  }


  // Check for interactions and Draw everything
  for ( auto &entity: ui_elements ) {
    Element &elem = ui_reg.get<Element>( entity );

    if ( !elem.enabled )
      continue;


    bool inside = CheckCollisionPointRec( GetMousePosition(), elem.transform );

    if ( !over_any_elem )
      over_any_elem = inside;

    bool interactive =
      ( elem.type == Type::TEXT_BUTTON || elem.type == Type::TEXTURE_BUTTON );

    // Check for interactions
    if ( DoInteraction(
           entity,
           inside,
           interactive,
           mouseWentUp,
           mouseWentDown ) ) {
      printf( "Interaction found! %d\n", elem.type );

      switch ( elem.type ) {
        case Type::TEXT_BUTTON: {
          TextButton button = ui_reg.get<TextButton>( entity );
          button.action();
        } break;
        case Type::TEXTURE_BUTTON: {
          TextureButton button = ui_reg.get<TextureButton>( entity );
          button.action();
        } break;
      }
    }

    // DRAW
    DrawElement( texture_cache, entity, elem );
  }

  if ( !over_any_elem ) {
    context.hot = entt::null;
    context.active = entt::null;
  }

  DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
  DrawFPS( GetScreenWidth() - 100, 2 );

  // TODO make this a part of debug
  DrawRectangle( GetScreenWidth() - 200, 102, 200, 24.0f, BLACK );
  std::string foo = "hot: " + HotId();
  DrawText( foo.c_str(), GetScreenWidth() - 200, 102, 24.0f, RED );

  DrawRectangle( GetScreenWidth() - 200, 152, 200, 24.0f, BLACK );
  std::string bar = "active: " + ActiveId();
  DrawText( bar.c_str(), GetScreenWidth() - 200, 152, 24.0f, RED );


  // DrawTexturePro(texture_cache[hstr{"context_panel"}]->texture, {747, 448}, {}, {0, 0}, 0, WHITE);
}

inline void LayoutPanel( Panel &panel, entt::entity entity ) {
  f32 total_height = 0;
  f32 total_width = 0;
  f32 tallest_child = 0;
  f32 widest_child = 0;
  Element &panel_elem = ui_reg.get<Element>( entity );

  f32 end_of_last_x = panel_elem.transform.x;
  f32 end_of_last_y = panel_elem.transform.y;


  // For each child
  for ( auto &child: panel.children ) {
    Element &elem = ui_reg.get<Element>( child );

    ResizeElement( child, elem );

    if ( panel.children_axis == Axis::ROW ) {
      // 2. Set the child x position based on alignment style.
      switch ( panel.children_horiz_align ) {
        case Align::START: {
          elem.transform.x = end_of_last_x;
          end_of_last_x = elem.transform.x + elem.transform.width;
        } break;
        case Align::SPACE_OUT: {
        } break;
      }

      // 3. Set the child y position based on alignment style.
      switch ( panel.children_vert_align ) {
        case Align::START: {
          elem.transform.y = panel_elem.transform.y;
        } break;
      }


    } else if ( panel.children_axis == Axis::COLUMN ) {
      // 2. Set the child x position based on alignment style.
      switch ( panel.children_horiz_align ) {
        case Align::START: {
          elem.transform.x = panel_elem.transform.x;
        } break;
        case Align::SPACE_OUT: {
        } break;
      }

      // 3. Set the child y position based on alignment style.
      switch ( panel.children_vert_align ) {
        case Align::START: {
          elem.transform.y = end_of_last_y + elem.margins.top;
          end_of_last_y =
            elem.transform.y + elem.transform.height + elem.margins.bottom;
        } break;
      }
    }
  }

  for ( auto &child: panel.children ) {
    rect &elem = ui_reg.get<Element>( child ).transform;

    total_width += elem.width;
    total_height += elem.height;

    if ( elem.width > widest_child )
      widest_child = elem.width;

    if ( elem.height > tallest_child )
      tallest_child = elem.height;
  }

  if ( !ui_reg.all_of<BasePanel>( entity ) ) {
    if ( panel.children_axis == Axis::ROW ) {
      panel_elem.transform.width = total_width;
      panel_elem.transform.height = tallest_child;
    } else if ( panel.children_axis == Axis::COLUMN ) {
      panel_elem.transform.width = widest_child;
      panel_elem.transform.height = total_height;
    }
  }
}

inline void ResizeElement( entt::entity entity, Element &elem ) {
  // TODO maybe these resize-cases should just be update methods on the structs
  switch ( elem.type ) {
    case Type::PANEL: {
      LayoutPanel( ui_reg.get<Panel>( entity ), entity );
    } break;
    case Type::TEXT_LABEL: {
      TextLabel &label = ui_reg.get<TextLabel>( entity );

      const vec2 text_dims = MeasureTextEx(
        Global::font_cache[hstr{ "font_romulus" }]->font,
        label.text.c_str(),
        label.font_size,
        2.0f );

      elem.transform.width = text_dims.x;
      elem.transform.height = text_dims.y;
    } break;
    case Type::TEXT_BUTTON: {
      TextButton &button = ui_reg.get<TextButton>( entity );

      const vec2 text_dims = MeasureTextEx(
        Global::font_cache[hstr{ "font_romulus" }]->font,
        button.text.c_str(),
        button.font_size,
        2.0f );

      elem.transform.width = text_dims.x;
      elem.transform.height = text_dims.y;
    } break;
    case Type::TEXTURE_BUTTON: {
      Texture2D texture = ui_reg.get<TextureButton>( entity ).texture;
      elem.transform.width = texture.width * UI::SCALE;
      elem.transform.height = texture.height * UI::SCALE;
    } break;
  }
}

inline bool DoInteraction(
  entt::entity entity,
  bool inside,
  bool interactive,
  bool mouseWentUp,
  bool mouseWentDown ) {
  bool result = false;
  // std::cout << "hot " << HotId() << " active " << ActiveId() << '\n';

  if ( entity == context.active ) {
    if ( mouseWentUp ) {
      if ( entity == context.hot )
        result = true;// do the button action

      context.active = entt::null;
    }
  } else if ( entity == context.hot ) {
    // if ( mouseWentDown && interactive ) // TODO might want to readd this
    if ( mouseWentDown )
      context.active = entity;
  }

  if ( inside ) {
    if ( context.active == entt::null ) {
      context.hot = entity;
      if ( mouseWentDown && interactive )
        context.active = entity;
    }
  }

  return result;
}

inline void
DrawElement( TextureCache &texture_cache, entt::entity entity, Element &elem ) {
  switch ( elem.type ) {
    case Type::BASE_PANEL: {
      auto &panel = ui_reg.get<BasePanel>( entity );
      DrawRectangleV(
        { elem.transform.x, elem.transform.y },
        { elem.transform.width, elem.transform.height },
        panel.background );
    } break;
    case Type::PANEL: {
      auto &panel = ui_reg.get<Panel>( entity );
      DrawRectangleV(
        { elem.transform.x, elem.transform.y },
        { elem.transform.width, elem.transform.height },
        panel.background );
    } break;
    case Type::TEXT_LABEL: {
      auto &label = ui_reg.get<TextLabel>( entity );
      DrawRectangleV(
        { elem.transform.x, elem.transform.y },
        { elem.transform.width, elem.transform.height },
        label.background );

      DrawTextEx(
        Global::font_cache[hstr{ "font_romulus" }]->font,
        label.text.c_str(),
        {
          elem.transform.x,
          elem.transform.y,
        },
        label.font_size,
        2.0,
        label.text_color );
    } break;
    case Type::TEXTURE_BUTTON: {
      auto &button = ui_reg.get<TextureButton>( entity );
      DrawTextureEx(
        button.texture,
        { elem.transform.x, elem.transform.y },
        0.0,
        SCALE,
        WHITE );
    } break;

    case Type::TEXT_BUTTON: {
      auto &button = ui_reg.get<TextButton>( entity );
      DrawRectangleV(
        { elem.transform.x, elem.transform.y },
        { elem.transform.width, elem.transform.height },
        button.background );

      DrawTextEx(
        Global::font_cache[hstr{ "font_romulus" }]->font,
        button.text.c_str(),
        {
          elem.transform.x,
          elem.transform.y + ( 0.5f * elem.transform.height ),
        },
        button.font_size,
        2.0,
        button.text_color );
    } break;
  }
}

inline void SelectListener( entt::registry &game_reg, entt::entity entity ) {
  printf( "SelectListener?\n" );
  if ( game_reg.all_of<Province::Component>( entity ) ) {

    auto context_panel = ui_lookup.at( "settlement_context_panel" );
    ToggleElement( context_panel, true );
    // Element &elem = ui_reg.get<Element>( context_panel );
    // elem.enabled = true;
  } else if ( game_reg.all_of<Actor::Component>( entity ) ) {
    auto actor = game_reg.get<Actor::Component>( entity );
    printf( "Actor: %s \n", actor.name );

    auto context_panel = ui_lookup.at( "actor_context_panel" );
    ToggleElement( context_panel, true );
    // entt::entity context_label = ui_lookup.at( "context_label" );
    // ui_reg.get<TextLabel>( context_label ).text = actor.name;
  }
}

inline void DeSelectListener() {
  printf( "DeSelectListener?\n" );
  auto context_panel = ui_lookup.at( "settlement_context_panel" );
  // TODO not deselecting properly?
  ToggleElement( context_panel, false );
}

inline void ToggleElement( entt::entity entity, bool on ) {
  Element &elem = ui_reg.get<Element>( entity );
  elem.enabled = on;
  if ( elem.type != Type::PANEL && elem.type != Type::BASE_PANEL )
    return;

  if ( ui_reg.any_of<BasePanel>( entity ) ) {
    BasePanel panel = ui_reg.get<BasePanel>( entity );
    for ( auto child: panel.children ) {
      ToggleElement( child, on );
    }

  } else if ( ui_reg.any_of<Panel>( entity ) ) {
    Panel panel = ui_reg.get<Panel>( entity );
    for ( auto child: panel.children ) {
      ToggleElement( child, on );
    }
  }
}

inline std::string ActiveId() {
  if ( context.active == entt::null )
    return "-1";
  else
    return std::to_string( (std::uint32_t) context.active );
}

inline std::string HotId() {
  if ( context.hot == entt::null )
    return "-1";
  else
    return std::to_string( (std::uint32_t) context.hot );
}

inline bool MouseIsOverUI() {
  // This is almost sufficent, but we need to account for panels too
  // not just items that can be active
  return context.active != entt::null || context.hot != entt::null;
}
};// namespace UI
