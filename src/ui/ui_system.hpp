#pragma once

#include <raylib.h>
#include <raymath.h>

#include "../components/actor.hpp"
#include "../components/event.hpp"
#include "../components/selected.hpp"
#include "../components/settlement.hpp"
#include "../renderer/fonts.hpp"
#include "../renderer/textures.hpp"
#include "../systems/selection_system.hpp"
#include "content/campaign_ui.hpp"
#include "content/main_menu_ui.hpp"
#include "content/modal_menu_ui.hpp"
#include "ui_lookups.hpp"

namespace UI {

inline Context context = { entt::null, entt::null };
inline std::vector<entt::entity> content;

inline void DisableCurrentContent();
inline void Draw();
inline void RecursiveDraw( Panel & );
inline void RecursiveLayout( Panel & );
inline void RecursiveInteractions( Panel &, bool &, bool, bool );
inline bool DoInteraction( entt::entity, bool, bool, bool, bool );
inline void ListenForSelect( entt::registry &, entt::entity );
inline void ListenForDeselect();
inline void RecursiveToggle( entt::entity, bool );
inline void SetContextNull();

inline void InitMainMenuUI() {
  DisableCurrentContent();
  content = CreateMainMenuUI();
  for ( entt::entity base: content ) {
    RecursiveToggle( base, true );
  }
}

inline void InitCampaignUI() {
  DisableCurrentContent();
  content = CreateCampaignUI();

  Global::world.on_construct<Selected::Component>().connect<&ListenForSelect>();
  Global::world.on_destroy<Selected::Component>().connect<&ListenForDeselect>();
}

inline void DisableCurrentContent() {
  for ( entt::entity base: content ) {
    RecursiveToggle( base, false );
  }
}

inline void UpdateOnFrame() {
  vec2 mousePos = GetMousePosition();
  bool mouseWentUp = IsMouseButtonReleased( 0 );
  bool mouseWentDown = IsMouseButtonPressed( 0 );
  bool mouseHeldDown = IsMouseButtonDown( 0 );
  bool over_any_elem = false;
  const f32 screen_width = GetScreenWidth();
  const f32 screen_height = GetScreenHeight();

  if ( screen_width > 1920 ) {
    SCALE = 2.0;
  } else if ( screen_width > 2560 ) {
    SCALE = 3.0;
  } else if ( screen_width >= 3840 ) {
    SCALE = 4.0;
  }

  for ( entt::entity base: content ) {
    Panel &panel = Get<Panel>( base );
    RecursiveLayout( panel );
    RecursiveInteractions( panel, over_any_elem, mouseWentUp, mouseWentDown );
  }

  // if ( Global::program_mode == Global::ProgramMode::ModalMenu ) {
  //   auto modal_menu = lookup["modal_menu"];
  //   Panel &panel = Get<Panel>( modal_menu );
  //   RecursiveLayout( panel );
  //   RecursiveInteractions( panel, over_any_elem, mouseWentUp, mouseWentDown );
  // } else {
  //   for ( entt::entity base: content ) {
  //     Panel &panel = Get<Panel>( base );
  //     RecursiveLayout( panel );
  //     RecursiveInteractions( panel, over_any_elem, mouseWentUp, mouseWentDown );
  //   }
  // }
}

inline void Draw() {
  // if ( Global::program_mode == Global::ProgramMode::ModalMenu ) {
  //   auto modal_menu = lookup["modal_menu"];
  //   RecursiveDraw( Get<Panel>( modal_menu ) );
  // } else {
  //   for ( entt::entity base: content ) {
  //     Panel &panel = Get<Panel>( base );
  //     RecursiveDraw( panel );
  //   }
  // }
  //
  for ( entt::entity base: content ) {
    Panel &panel = Get<Panel>( base );
    RecursiveDraw( panel );
  }


  DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
  DrawFPS( GetScreenWidth() - 100, 2 );

  DrawRectangle( GetScreenWidth() - 600, 102, 200, 24.0f, BLACK );
  std::string foo = "hot: " + EntityIdToString( context.hot );
  DrawText( foo.c_str(), GetScreenWidth() - 600, 102, 24.0f, RED );

  DrawRectangle( GetScreenWidth() - 600, 152, 200, 24.0f, BLACK );
  std::string bar = "active: " + EntityIdToString( context.active );
  DrawText( bar.c_str(), GetScreenWidth() - 600, 152, 24.0f, RED );

  DrawRectangle( GetScreenWidth() - 600, 202, 200, 24.0f, BLACK );
  std::string selected_ent =
    "entity: " + EntityIdToString( SelectionSystem::selected_entity );
}

inline void RecursiveLayout( Panel &parent_panel ) {
  f32 total_height = 0;
  f32 total_width = 0;
  f32 tallest_child = 0;
  f32 widest_child = 0;
  f32 end_of_last_x = parent_panel.elem.transform.x;
  f32 end_of_last_y = parent_panel.elem.transform.y;

  // NOTE: might have to go after the enabled check?
  parent_panel.Place();
  parent_panel.Resize();

  if ( !parent_panel.elem.enabled )
    return;

  for ( entt::entity child: parent_panel.children ) {

    switch ( GetType( child ) ) {
      case Type::Panel: {
        Panel &child_panel = Get<Panel>( child );
        RecursiveLayout( child_panel );
      } break;
      case Type::TextLabel: {
        Get<TextLabel>( child ).Resize();
      } break;
      case Type::TextButton: {
        Get<TextButton>( child ).label.Resize();
      } break;
      case Type::TextureLabel: {
        Get<TextureLabel>( child ).Resize();
      } break;
      case Type::TextureButton: {
        Get<TextureButton>( child ).label.Resize();
      } break;
      default:
        break;
    }
    // TODO ^ these could probably be consolidated using a template
    rect &transform = GetTransform( child );

    if ( parent_panel.children_axis == Axis::ROW ) {
      // 2. Set the child x position based on alignment style.
      switch ( parent_panel.children_horiz_align ) {
        case Align::START: {
          transform.x = end_of_last_x;
          end_of_last_x = transform.x + transform.width;
        } break;
        case Align::SPACE_OUT: {
        } break;
      }

      // 3. Set the child y position based on alignment style.
      switch ( parent_panel.children_vert_align ) {
        case Align::START: {
          transform.y = parent_panel.elem.transform.y;
        } break;
      }
    } else if ( parent_panel.children_axis == Axis::COLUMN ) {
      // 2. Set the child x position based on alignment style.
      switch ( parent_panel.children_horiz_align ) {
        case Align::START: {
          transform.x = parent_panel.elem.transform.x;
        } break;
        case Align::SPACE_OUT: {
        } break;
      }

      // 3. Set the child y position based on alignment style.
      switch ( parent_panel.children_vert_align ) {
        case Align::START: {
          transform.y = end_of_last_y;
          // + elem.margins.top;
          end_of_last_y = transform.y + transform.height;
          // + elem.margins.bottom;
        } break;
      }
    }
  }

  if ( !parent_panel.abs_size ) {
    for ( entt::entity child: parent_panel.children ) {
      rect &transform = GetTransform( child );

      total_width += transform.width;
      total_height += transform.height;

      if ( transform.width > widest_child )
        widest_child = transform.width;

      if ( transform.height > tallest_child )
        tallest_child = transform.height;
    }

    if ( parent_panel.children_axis == Axis::ROW ) {
      parent_panel.elem.transform.width = total_width;
      parent_panel.elem.transform.height = tallest_child;
    } else if ( parent_panel.children_axis == Axis::COLUMN ) {
      parent_panel.elem.transform.width = widest_child;
      parent_panel.elem.transform.height = total_height;
    }
  }
}

inline void RecursiveDraw( Panel &panel ) {
  if ( !panel.elem.enabled )
    return;

  panel.Draw();

  for ( entt::entity child: panel.children ) {
    switch ( GetType( child ) ) {
      case Type::Panel: {
        RecursiveDraw( Get<Panel>( child ) );
      } break;
      case Type::TextLabel: {
        Get<TextLabel>( child ).Draw();
      } break;
      case Type::TextButton: {
        Get<TextButton>( child ).Draw();
      } break;
      case Type::TextureLabel: {
        Get<TextureLabel>( child ).Draw();
      } break;
      case Type::TextureButton: {
        Get<TextureButton>( child ).Draw();
      } break;
      default:
        break;
    }
  }
}

inline void RecursiveInteractions(
  Panel &panel,
  bool &over_any_elem,
  bool mouseWentUp,
  bool mouseWentDown
) {

  if ( !panel.elem.enabled )
    return;

  for ( auto &child: panel.children ) {
    if ( !IsEnabled( child ) )
      continue;

    if ( Has<Panel>( child ) ) {
      RecursiveInteractions(
        Get<Panel>( child ),
        over_any_elem,
        mouseWentUp,
        mouseWentDown
      );
    }

    UpdateElem( child );

    rect &transform = GetTransform( child );

    bool inside = CheckCollisionPointRec( GetMousePosition(), transform );

    if ( !over_any_elem )
      over_any_elem = inside;

    if ( DoInteraction(
           child,
           inside,
           IsInteractive( child ),
           mouseWentUp,
           mouseWentDown
         ) ) {
      std::cout << "INTERACTION DETECTED!!!" << std::endl;

      if ( IsClickable( child ) ) {
        action_lookup.at( GetId( child ) )();
      }
    }
  }

  if ( !over_any_elem ) {
    SetContextNull();
  }
}

inline bool DoInteraction(
  entt::entity entity,
  bool inside,
  bool interactive,
  bool mouseWentUp,
  bool mouseWentDown
) {
  bool result = false;

  if ( entity == context.active ) {
    if ( mouseWentUp ) {
      if ( entity == context.hot )
        result = true;// do the button action

      context.active = entt::null;
    }
  } else if ( entity == context.hot ) {
    // if ( mouseWentDown && interactive )
    if ( mouseWentDown && interactive )
      context.active = entity;
  }

  if ( inside ) {
    if ( context.active == entt::null && interactive ) {
      context.hot = entity;
      if ( mouseWentDown && interactive )
        context.active = entity;
    }
  }

  return result;
}


// TODO replace this param reg with the global one
inline void ListenForSelect( entt::registry &game_reg, entt::entity entity ) {
  printf( "SelectListener?\n" );
  if ( game_reg.all_of<Province::Component>( entity ) ) {
    auto &context_panel = lookup.at( "settlement_context_panel" );
    RecursiveToggle( context_panel, true );
  } else if ( game_reg.all_of<Actor::Component>( entity ) ) {
    auto actor = game_reg.get<Actor::Component>( entity );

    auto context_panel = lookup.at( "actor_context_panel" );
    RecursiveToggle( context_panel, true );
  }
}

inline void ListenForDeselect() {
  printf( "DeSelectListener?\n" );
  auto context_panel = lookup.at( "settlement_context_panel" );
  RecursiveToggle( context_panel, false );
  context_panel = lookup.at( "actor_context_panel" );
  RecursiveToggle( context_panel, false );
  SetContextNull();
}

inline void RecursiveToggle( entt::entity entity, bool on ) {
  ToggleElem( entity, on );

  if ( !Has<Panel>( entity ) )
    return;

  for ( entt::entity child: Global::local.get<Panel>( entity ).children ) {
    RecursiveToggle( child, on );
  }
}


inline bool MouseIsOverUI() {
  // This is almost sufficent, but we need to account for panels too
  // not just items that can be active?
  return context.active != entt::null || context.hot != entt::null;
}

inline void SetContextNull() {
  context.hot = entt::null;
  context.active = entt::null;
}

inline void SetModalMenu( bool on ) {
  RecursiveToggle( lookup.at( "modal_menu" ), on );
  if ( !on )
    SetContextNull();
}

};// namespace UI
