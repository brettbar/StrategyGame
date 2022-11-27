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
#include "ui_content.hpp"
#include "ui_lookups.hpp"
#include "widgets/actor_context_panel.hpp"
#include "widgets/modal_menu.hpp"
#include "widgets/overview_banner.hpp"
#include "widgets/settlement_context_panel.hpp"

namespace UI {

inline Context context = { entt::null, entt::null };

inline void Init( TextureCache & );
inline void Draw();
inline void RecursiveDraw( Panel & );
inline void RecursiveLayout( Panel & );
inline void RecursiveInteractions( Panel &, bool &, bool, bool );
inline bool DoInteraction( entt::entity, bool, bool, bool, bool );
inline void ListenForSelect( entt::registry &, entt::entity );
inline void ListenForDeselect();
inline void RecursiveToggle( entt::entity, bool );

inline void Init( TextureCache &texture_cache ) {
  // OverviewBanner();
  // ModalMenu();
  content = CreateContent();

  Global::world.on_construct<Selected::Component>().connect<&ListenForSelect>();
  Global::world.on_destroy<Selected::Component>().connect<&ListenForDeselect>();
}

// inline void ToggleModalMenu() {
//   auto elem = Global::local.get<Element>( lookup.at( "modal_menu" ) );
//   elem.enabled = !elem.enabled;
// }

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
}

inline void Draw() {
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
        Get<TextButton>( child ).label.Draw();
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

    // UpdateElem( child );

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
      if ( IsClickable( child ) ) {
        std::cout << "INTERACTION DETECTED!!!" << std::endl;
        action_lookup.at( GetId( child ) )();
      }
    }
  }

  if ( !over_any_elem ) {
    context.hot = entt::null;
    context.active = entt::null;
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


// TODO replace this param reg with the global one
inline void ListenForSelect( entt::registry &game_reg, entt::entity entity ) {
  printf( "SelectListener?\n" );
  if ( game_reg.all_of<Province::Component>( entity ) ) {
    auto &context_panel = lookup.at( "settlement_context_panel" );
    RecursiveToggle( context_panel, true );
    // Element &elem = Global::ui_reg.get<Element>( context_panel );
    // elem.enabled = true;
  } else if ( game_reg.all_of<Actor::Component>( entity ) ) {
    auto actor = game_reg.get<Actor::Component>( entity );
    printf( "Actor: %s \n", actor.name );

    auto context_panel = lookup.at( "actor_context_panel" );
    RecursiveToggle( context_panel, true );
    //
    // entt::entity context_label = ui_lookup.at( "context_label" );
    // Global::ui_reg.get<TextLabel>( context_label ).text = actor.name;
  }
}

inline void ListenForDeselect() {
  printf( "DeSelectListener?\n" );
  auto context_panel = lookup.at( "settlement_context_panel" );
  RecursiveToggle( context_panel, false );
  context_panel = lookup.at( "actor_context_panel" );
  RecursiveToggle( context_panel, false );
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
  // not just items that can be active
  return context.active != entt::null || context.hot != entt::null;
}
};// namespace UI


/*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*
*/

// inline void LayoutPanel( Panel &panel ) {

// for ( auto &child: panel.children ) {
// ResizeElement( elem );

// if ( panel.children_axis == Axis::ROW ) {
//   // 2. Set the child x position based on alignment style.
//   switch ( panel.children_horiz_align ) {
//     case Align::START: {
//       elem.transform.x = end_of_last_x;
//       end_of_last_x = elem.transform.x + elem.transform.width;
//     } break;
//     case Align::SPACE_OUT: {
//     } break;
//   }

//   // 3. Set the child y position based on alignment style.
//   switch ( panel.children_vert_align ) {
//     case Align::START: {
//       elem.transform.y = panel_elem.transform.y;
//     } break;
//   }
// } else if ( panel.children_axis == Axis::COLUMN ) {
//   // 2. Set the child x position based on alignment style.
//   switch ( panel.children_horiz_align ) {
//     case Align::START: {
//       elem.transform.x = panel_elem.transform.x;
//     } break;
//     case Align::SPACE_OUT: {
//     } break;
//   }

//   // 3. Set the child y position based on alignment style.
//   switch ( panel.children_vert_align ) {
//     case Align::START: {
//       elem.transform.y = end_of_last_y + elem.margins.top;
//       end_of_last_y =
//         elem.transform.y + elem.transform.height + elem.margins.bottom;
//     } break;
//   }
// }
// for ( auto &child: panel.children ) {
//   rect &elem = Global::local.get<Element>( child ).transform;

//   total_width += elem.width;
//   total_height += elem.height;

//   if ( elem.width > widest_child )
//     widest_child = elem.width;

//   if ( elem.height > tallest_child )
//     tallest_child = elem.height;
// }

// if ( !Has<BasePanel>( entity ) ) {
//   if ( panel.children_axis == Axis::ROW ) {
//     panel_elem.transform.width = total_width;
//     panel_elem.transform.height = tallest_child;
//   } else if ( panel.children_axis == Axis::COLUMN ) {
//     panel_elem.transform.width = widest_child;
//     panel_elem.transform.height = total_height;
//   }
// }
// }


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
//   //     assert( Has<Panel>( child ) );

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

//   // if ( !Has<BasePanel>( entity ) ) {
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
