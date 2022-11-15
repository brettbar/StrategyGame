#pragma once

#include <raylib.h>
#include <raymath.h>

#include "../components/actor.hpp"
#include "../components/event.hpp"
#include "../components/selected.hpp"
#include "../components/settlement.hpp"
#include "../events.hpp"
#include "../renderer/fonts.hpp"
#include "../renderer/textures.hpp"
#include "../systems/selection_system.hpp"
#include "ui_components.hpp"
#include "ui_lookups.hpp"
#include "widgets/actor_context_panel.hpp"
#include "widgets/modal_menu.hpp"
#include "widgets/overview_banner.hpp"
#include "widgets/settlement_context_panel.hpp"

namespace UI {

inline Context context;

inline std::vector<Panel> content;

inline void Init( TextureCache & );
inline void Draw();
inline void LayoutPanel( Panel &, entt::entity );
inline void ResizeElement( entt::entity, Element & );
inline bool DoInteraction( entt::entity, bool, bool, bool, bool );
inline void DrawElement( TextureCache &, entt::entity, Element & );
inline void ListenForSelect( entt::registry &, entt::entity );
inline void ListenForDeselect();
inline void ToggleElement( entt::entity, bool );

inline void Init( TextureCache &texture_cache ) {
  // OverviewBanner();

  content = {
    Panel(
      "context_panel",
      Fade( BLACK, 0.5 ),
      Axis::ROW,
      Align::START,
      Align::START,
      Panel(
        "context_tab_group",
        Fade( BLACK, 0.5 ),
        Axis::COLUMN,
        Align::START,
        Align::START,
        {}
      )
    ),
  };

  // SettlementContextPanel(
  //   { SettlementContextTabGroup( {
  //       SettlementContextTabButton( "context_tab_overview" ),
  //       SettlementContextTabButton( "context_tab_population" ),
  //       SettlementContextTabButton( "context_tab_resources" ),
  //       SettlementContextTabButton( "context_tab_culture" ),
  //       SettlementContextTabButton( "context_tab_religion" ),
  //       SettlementContextTabButton( "context_tab_construction" ),
  //       SettlementContextTabButton( "context_tab_garrison" ),
  //     } ),
  //     SettlementContextContent( { SettlementContent( {
  //       SettlementName(),
  //       SettlementPopulation(),
  //       SettlementDevelopment(),
  //     } ) } ) }
  // );

  // SettlementContextPanel(
  //   { SettlementContextTabGroup( {
  //       SettlementContextTabButton( "context_tab_overview" ),
  //       SettlementContextTabButton( "context_tab_population" ),
  //       SettlementContextTabButton( "context_tab_resources" ),
  //       SettlementContextTabButton( "context_tab_culture" ),
  //       SettlementContextTabButton( "context_tab_religion" ),
  //       SettlementContextTabButton( "context_tab_construction" ),
  //       SettlementContextTabButton( "context_tab_garrison" ),
  //     } ),
  //     SettlementContextContent( { SettlementContent( {
  //       SettlementName(),
  //       SettlementPopulation(),
  //       SettlementDevelopment(),
  //     } ) } ) }
  // );

  // ActorContextPanel( { ActorActionsPanel( {
  //   ActorSpawnSettlementButton(),
  // } ) } );
  // ModalMenu();

  Global::world.on_construct<Selected::Component>().connect<&ListenForSelect>();
  Global::world.on_destroy<Selected::Component>().connect<&ListenForDeselect>();
}// namespace UI

// inline void ToggleModalMenu() {
//   auto elem = Global::local.get<Element>( lookup.at( "modal_menu" ) );
//   elem.enabled = !elem.enabled;
// }

inline void Update() {}


inline void Draw() {
  for ( Panel panel: content ) {
    if ( !panel.enabled )
      continue;

    // parent.update( elem, parent );

    LayoutPanel( parent, ent );

    for ( auto &child: parent.children ) {
      Element &elem = Global::local.get<Element>( child );
      Panel &panel = Global::local.get<Panel>( child );
      if ( !elem.enabled )
        continue;

      LayoutPanel( panel, child );
    }
  }
}

// inline void Draw() {
//   vec2 mousePos = GetMousePosition();
//   bool mouseWentUp = IsMouseButtonReleased( 0 );
//   bool mouseWentDown = IsMouseButtonPressed( 0 );
//   bool mouseHeldDown = IsMouseButtonDown( 0 );
//   bool over_any_elem = false;
//   const f32 screen_width = GetScreenWidth();
//   const f32 screen_height = GetScreenHeight();

//   // auto ui_elements = Global::local.view<Element>();
//   // auto base_panels = Global::local.view<BasePanel>();

//   if ( screen_width > 1920 ) {
//     SCALE = 2.0;
//   } else if ( screen_width > 2560 ) {
//     SCALE = 3.0;
//   } else if ( screen_width >= 3840 ) {
//     SCALE = 4.0;
//   }


//   // for ( auto &ent: base_panels ) {
//   //   Element &elem = Global::local.get<Element>( ent );
//   //   BasePanel &parent = Global::local.get<BasePanel>( ent );

//   //   if ( !elem.enabled )
//   //     continue;

//   //   // 0. Resize the parent panel.
//   //   parent.update( elem, parent );

//   //   LayoutPanel( parent, ent );

//   //   for ( auto &child: parent.children ) {
//   //     assert( Global::local.all_of<Panel>( child ) );

//   //     Element &elem = Global::local.get<Element>( child );
//   //     Panel &panel = Global::local.get<Panel>( child );
//   //     if ( !elem.enabled )
//   //       continue;

//   //     LayoutPanel( panel, child );
//   //   }
//   // }

//   // // Check for interactions and Draw everything
//   // for ( auto &entity: ui_elements ) {
//   //   Element &elem = Global::local.get<Element>( entity );

//   //   if ( !elem.enabled )
//   //     continue;

//   //   if ( Global::program_mode == Global::ProgramMode::MODAL_MENU ) {
//   //     if ( elem.id != "modal_menu" ) {
//   //       continue;
//   //     }
//   //   }

//   //   bool inside = CheckCollisionPointRec( GetMousePosition(), elem.transform );

//   //   if ( !over_any_elem )
//   //     over_any_elem = inside;

//   //   bool interactive =
//   //     ( elem.type == Type::TextButton || elem.type == Type::TextureButton );

//   //   // UPDATE
//   //   switch ( elem.type ) {
//   //     case Type::TextLabel: {
//   //       TextLabel &label = Global::local.get<TextLabel>( entity );
//   //       if ( label.dynamic )
//   //         label.text = update_lookup.at( elem.id )();
//   //     } break;
//   //     case Type::TextButton: {
//   //       TextButton &button = Global::local.get<TextButton>( entity );
//   //       if ( button.dynamic )
//   //         button.text = update_lookup.at( elem.id )();

//   //       button.clickable = clickable_lookup.at( elem.id )();
//   //     } break;
//   //     case Type::TextureButton: {
//   //       TextureButton &button = Global::local.get<TextureButton>( entity );
//   //       button.clickable = clickable_lookup.at( elem.id )();
//   //     } break;
//   //   }

//   //   // Check for interactions
//   //   if ( DoInteraction(
//   //          entity,
//   //          inside,
//   //          interactive,
//   //          mouseWentUp,
//   //          mouseWentDown
//   //        ) ) {
//   //     switch ( elem.type ) {
//   //       case Type::TextButton: {
//   //         TextButton &button = Global::local.get<TextButton>( entity );

//   //         if ( button.clickable )
//   //           action_lookup.at( elem.id )();
//   //       } break;
//   //       case Type::TextureButton: {
//   //         TextureButton &button = Global::local.get<TextureButton>( entity );

//   //         if ( button.clickable )
//   //           action_lookup.at( elem.id )();
//   //       } break;
//   //     }
//   //   }

//   //   // DRAW
//   //   DrawElement( texture_cache, entity, elem );
//   // }

//   // if ( !over_any_elem ) {
//   //   context.hot = entt::null;
//   //   context.active = entt::null;
//   // }

//   // DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
//   // DrawFPS( GetScreenWidth() - 100, 2 );

//   // // TODO make this a part of debug
//   // DrawRectangle( GetScreenWidth() - 200, 102, 200, 24.0f, BLACK );
//   // std::string foo = "hot: " + EntityIdToString( context.hot );
//   // DrawText( foo.c_str(), GetScreenWidth() - 200, 102, 24.0f, RED );

//   // DrawRectangle( GetScreenWidth() - 200, 152, 200, 24.0f, BLACK );
//   // std::string bar = "active: " + EntityIdToString( context.active );
//   // DrawText( bar.c_str(), GetScreenWidth() - 200, 152, 24.0f, RED );

//   // DrawRectangle( GetScreenWidth() - 200, 202, 200, 24.0f, BLACK );
//   // std::string selected_ent =
//   //   "entity: " + EntityIdToString( SelectionSystem::selected_entity );
//   // DrawText( selected_ent.c_str(), GetScreenWidth() - 200, 202, 24.0f, GREEN );
// }

// inline void LayoutPanel( Panel &panel, entt::entity entity ) {
//   // f32 total_height = 0;
//   // f32 total_width = 0;
//   // f32 tallest_child = 0;
//   // f32 widest_child = 0;
//   // Element &panel_elem = Global::local.get<Element>( entity );

//   // f32 end_of_last_x = panel_elem.transform.x;
//   // f32 end_of_last_y = panel_elem.transform.y;

//   // // For each child
//   // for ( auto &child: panel.children ) {
//   //   Element &elem = Global::local.get<Element>( child );

//   //   ResizeElement( child, elem );

//   //   if ( panel.children_axis == Axis::ROW ) {
//   //     // 2. Set the child x position based on alignment style.
//   //     switch ( panel.children_horiz_align ) {
//   //       case Align::START: {
//   //         elem.transform.x = end_of_last_x;
//   //         end_of_last_x = elem.transform.x + elem.transform.width;
//   //       } break;
//   //       case Align::SPACE_OUT: {
//   //       } break;
//   //     }

//   //     // 3. Set the child y position based on alignment style.
//   //     switch ( panel.children_vert_align ) {
//   //       case Align::START: {
//   //         elem.transform.y = panel_elem.transform.y;
//   //       } break;
//   //     }
//   //   } else if ( panel.children_axis == Axis::COLUMN ) {
//   //     // 2. Set the child x position based on alignment style.
//   //     switch ( panel.children_horiz_align ) {
//   //       case Align::START: {
//   //         elem.transform.x = panel_elem.transform.x;
//   //       } break;
//   //       case Align::SPACE_OUT: {
//   //       } break;
//   //     }

//   //     // 3. Set the child y position based on alignment style.
//   //     switch ( panel.children_vert_align ) {
//   //       case Align::START: {
//   //         elem.transform.y = end_of_last_y + elem.margins.top;
//   //         end_of_last_y =
//   //           elem.transform.y + elem.transform.height + elem.margins.bottom;
//   //       } break;
//   //     }
//   //   }
//   // }

//   // for ( auto &child: panel.children ) {
//   //   rect &elem = Global::local.get<Element>( child ).transform;

//   //   total_width += elem.width;
//   //   total_height += elem.height;

//   //   if ( elem.width > widest_child )
//   //     widest_child = elem.width;

//   //   if ( elem.height > tallest_child )
//   //     tallest_child = elem.height;
//   // }

//   // if ( !Global::local.all_of<BasePanel>( entity ) ) {
//   //   if ( panel.children_axis == Axis::ROW ) {
//   //     panel_elem.transform.width = total_width;
//   //     panel_elem.transform.height = tallest_child;
//   //   } else if ( panel.children_axis == Axis::COLUMN ) {
//   //     panel_elem.transform.width = widest_child;
//   //     panel_elem.transform.height = total_height;
//   //   }
//   // }
// }

// inline void ResizeElement( entt::entity entity, Element &elem ) {
//   // TODO maybe these resize-cases should just be update methods on the structs
//   switch ( elem.type ) {
//     case Type::Panel: {
//       LayoutPanel( Global::local.get<Panel>( entity ), entity );
//     } break;
//     case Type::TextLabel: {
//       TextLabel &label = Global::local.get<TextLabel>( entity );

//       const vec2 text_dims = MeasureTextEx(
//         Global::font_cache[hstr{ "font_romulus" }]->font,
//         label.text.c_str(),
//         label.font_size,
//         2.0f
//       );

//       elem.transform.width = text_dims.x;
//       elem.transform.height = text_dims.y;
//     } break;
//     case Type::TextButton: {
//       TextButton &button = Global::local.get<TextButton>( entity );

//       const vec2 text_dims = MeasureTextEx(
//         Global::font_cache[hstr{ "font_romulus" }]->font,
//         button.text.c_str(),
//         button.font_size,
//         2.0f
//       );

//       elem.transform.width = text_dims.x;
//       elem.transform.height = text_dims.y;
//     } break;
//     case Type::TextureButton: {
//       Texture2D texture = Global::local.get<TextureButton>( entity ).texture;
//       elem.transform.width = texture.width * UI::SCALE;
//       elem.transform.height = texture.height * UI::SCALE;
//     } break;
//   }
// }

// inline bool DoInteraction(
//   entt::entity entity,
//   bool inside,
//   bool interactive,
//   bool mouseWentUp,
//   bool mouseWentDown
// ) {
//   bool result = false;
//   // std::cout << "hot " << HotId() << " active " << ActiveId() << '\n';

//   if ( entity == context.active ) {
//     if ( mouseWentUp ) {
//       if ( entity == context.hot )
//         result = true;// do the button action

//       context.active = entt::null;
//     }
//   } else if ( entity == context.hot ) {
//     // if ( mouseWentDown && interactive ) // TODO might want to readd this
//     if ( mouseWentDown )
//       context.active = entity;
//   }

//   if ( inside ) {
//     if ( context.active == entt::null ) {
//       context.hot = entity;
//       if ( mouseWentDown && interactive )
//         context.active = entity;
//     }
//   }

//   return result;
// }

// inline void
// DrawElement( TextureCache &texture_cache, entt::entity entity, Element &elem ) {
//   // switch ( elem.type ) {
//   //   case Type::BasePanel: {
//   //     auto &panel = Global::local.get<BasePanel>( entity );
//   //     DrawRectangleV(
//   //       { elem.transform.x, elem.transform.y },
//   //       { elem.transform.width, elem.transform.height },
//   //       panel.background
//   //     );
//   //   } break;
//   //   case Type::Panel: {
//   //     auto &panel = Global::local.get<Panel>( entity );
//   //     DrawRectangleV(
//   //       { elem.transform.x, elem.transform.y },
//   //       { elem.transform.width, elem.transform.height },
//   //       panel.background
//   //     );
//   //   } break;
//   //   case Type::TextLabel: {
//   //     auto &label = Global::local.get<TextLabel>( entity );
//   //     DrawRectangleV(
//   //       { elem.transform.x, elem.transform.y },
//   //       { elem.transform.width, elem.transform.height },
//   //       label.background
//   //     );

//   //     DrawTextEx(
//   //       Global::font_cache[hstr{ "font_romulus" }]->font,
//   //       label.text.c_str(),
//   //       {
//   //         elem.transform.x,
//   //         elem.transform.y,
//   //       },
//   //       label.font_size,
//   //       2.0,
//   //       label.text_color
//   //     );
//   //   } break;
//   //   case Type::TextureButton: {
//   //     auto &button = Global::local.get<TextureButton>( entity );
//   //     DrawTextureEx(
//   //       button.texture,
//   //       { elem.transform.x, elem.transform.y },
//   //       0.0,
//   //       SCALE,
//   //       WHITE
//   //     );
//   //   } break;

//   //   case Type::TextButton: {
//   //     auto &button = Global::local.get<TextButton>( entity );
//   //     Color background = button.background;

//   //     if ( !button.clickable ) {
//   //       background = Fade( BLACK, 0.5 );
//   //     }

//   //     DrawRectangleV(
//   //       { elem.transform.x, elem.transform.y },
//   //       { elem.transform.width, elem.transform.height },
//   //       background
//   //     );

//   //     DrawTextEx(
//   //       Global::font_cache[hstr{ "font_romulus" }]->font,
//   //       button.text.c_str(),
//   //       {
//   //         elem.transform.x,
//   //         elem.transform.y + ( 0.5f * elem.transform.height ),
//   //       },
//   //       button.font_size,
//   //       2.0,
//   //       button.text_color
//   //     );
//   //   } break;
//   // }
// }

// inline void ListenForSelect( entt::registry &game_reg, entt::entity entity ) {
//   printf( "SelectListener?\n" );
//   if ( game_reg.all_of<Province::Component>( entity ) ) {
//     auto context_panel = lookup.at( "settlement_context_panel" );
//     ToggleElement( context_panel, true );
//     // Element &elem = Global::ui_reg.get<Element>( context_panel );
//     // elem.enabled = true;
//   } else if ( game_reg.all_of<Actor::Component>( entity ) ) {
//     auto actor = game_reg.get<Actor::Component>( entity );
//     printf( "Actor: %s \n", actor.name );

//     auto context_panel = lookup.at( "actor_context_panel" );
//     ToggleElement( context_panel, true );
//     // entt::entity context_label = ui_lookup.at( "context_label" );
//     // Global::ui_reg.get<TextLabel>( context_label ).text = actor.name;
//   }
// }

// inline void ListenForDeselect() {
//   printf( "DeSelectListener?\n" );
//   auto context_panel = lookup.at( "settlement_context_panel" );
//   // TODO not deselecting properly?
//   ToggleElement( context_panel, false );
// }

// inline void ToggleElement( entt::entity entity, bool on ) {
//   // Element &elem = Global::local.get<Element>( entity );
//   // elem.enabled = on;
//   // if ( elem.type != Type::Panel && elem.type != Type::BasePanel )
//   //   return;

//   // if ( Global::local.all_of<BasePanel>( entity ) ) {
//   //   BasePanel panel = Global::local.get<BasePanel>( entity );
//   //   for ( auto child: panel.children ) {
//   //     ToggleElement( child, on );
//   //   }
//   // } else if ( Global::local.all_of<Panel>( entity ) ) {
//   //   Panel panel = Global::local.get<Panel>( entity );
//   //   for ( auto child: panel.children ) {
//   //     ToggleElement( child, on );
//   //   }
//   // }
// }

// inline bool MouseIsOverUI() {
//   // This is almost sufficent, but we need to account for panels too
//   // not just items that can be active
//   return context.active != entt::null || context.hot != entt::null;
// }
};// namespace UI
