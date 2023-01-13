#pragma once

#include "../world/components/actor.hpp"
#include "../world/components/event.hpp"
#include "../world/components/selected.hpp"
#include "../world/components/settlement.hpp"

#include "../shared/fonts.hpp"
#include "../shared/textures.hpp"
#include "../world/systems/selection_system.hpp"

#include "content/campaign_ui.hpp"
#include "content/faction_select_menu.hpp"
#include "content/lobby_ui.hpp"
#include "content/main_menu_ui.hpp"
#include "content/modal_menu_ui.hpp"


#include "ui_utils.hpp"

namespace UI {

  inline Context context = { entt::null, entt::null };
  inline std::vector<entt::entity> _content;

  inline void EnableContent();
  inline void DisableCurrentContent();

  inline void RecursiveLayout( Panel & );
  inline void RecursiveInteractions( Panel &, bool &, bool, bool );

  inline void RecursiveDraw( Panel & );
  inline void Draw();

  inline bool DoInteraction( entt::entity, bool, bool, bool, bool );
  inline void ListenForSelect( entt::registry &, entt::entity );
  inline void ListenForDeselect();
  inline void SetContextNull();

  // TOOD these Enable functions can probably be consolidated

  inline void EnableMainMenuUI() {
    DisableCurrentContent();
    _content = CreateMainMenuUI();
    EnableContent();
  }

  inline void EnableCampaignUI() {
    DisableCurrentContent();
    _content = CreateCampaignUI();

    Global::world.on_construct<Selected::Component>().connect<&ListenForSelect>(
    );
    Global::world.on_destroy<Selected::Component>().connect<&ListenForDeselect>(
    );
  }

  inline void EnableModalMenuUI() {
    DisableCurrentContent();
    _content = CreateModalMenuUI();
    EnableContent();
  }

  inline void EnableFactionSelectMenuUI() {
    DisableCurrentContent();
    _content = CreateFactionSelectMenuUI();
    EnableContent();
  }

  inline void EnableLobby() {
    DisableCurrentContent();
    _content = CreateLobbyUI();
    EnableContent();
  }

  inline void EnableContent() {
    for ( entt::entity base: _content ) {
      RecursiveToggle( base, true );
    }
  }

  inline void DisableCurrentContent() {
    SetContextNull();
    for ( entt::entity base: _content ) {
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
    }
    else if ( screen_width > 2560 ) {
      SCALE = 3.0;
    }
    else if ( screen_width >= 3840 ) {
      SCALE = 4.0;
    }

    for ( entt::entity base: _content ) {
      Panel &panel = Get<Panel>( base );
      RecursiveLayout( panel );
      RecursiveInteractions( panel, over_any_elem, mouseWentUp, mouseWentDown );
    }
  }

  inline void Draw() {
    for ( entt::entity base: _content ) {
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
    DrawText( selected_ent.c_str(), GetScreenWidth() - 600, 202, 24.0f, RED );


    DrawRectangle( GetScreenWidth() - 600, 252, 200, 24.0f, BLACK );
    DrawText(
      std::to_string( Global::state.timeScale ).c_str(),
      GetScreenWidth() - 600,
      252,
      24.0f,
      WHITE
    );
  }

  inline void RecursiveLayout( Panel &parent_panel ) {
    f32 total_height = 0;
    f32 total_width = 0;
    f32 tallest_child = 0;
    f32 widest_child = 0;
    f32 end_of_last_x = parent_panel.elem.transform.x;
    f32 end_of_last_y = parent_panel.elem.transform.y;

    // NOTE: might have to go after the enabled check?
    // parent_panel.Place();
    // parent_panel.Resize();
    parent_panel.Update();

    if ( !parent_panel.elem.enabled )
      return;

    for ( entt::entity child: parent_panel.children ) {

      switch ( GetType( child ) ) {
        case Type::Panel: {
          Panel &child_panel = Get<Panel>( child );
          RecursiveLayout( child_panel );
        } break;
        case Type::StackPanel: {

          StackPanel &child_panel = Get<StackPanel>( child );

          entt::entity single_child =
            child_panel.children[child_panel.curr_index];

          assert( Has<Panel>( single_child ) );
          Panel &single_child_panel = Get<Panel>( single_child );
          single_child_panel.elem.transform.x = child_panel.elem.transform.x;
          single_child_panel.elem.transform.y = child_panel.elem.transform.y;

          RecursiveLayout( Get<Panel>( single_child ) );

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

      if ( parent_panel.children_axis == Axis::Row ) {
        // 2. Set the child x position based on alignment style.
        switch ( parent_panel.children_horiz_align ) {
          case Align::Start: {
            transform.x = end_of_last_x;
            end_of_last_x = transform.x + transform.width;
          } break;
          case Align::SpaceOut: {
          } break;
        }

        // 3. Set the child y position based on alignment style.
        switch ( parent_panel.children_vert_align ) {
          case Align::Start: {
            transform.y = parent_panel.elem.transform.y;
          } break;
        }
      }
      else if ( parent_panel.children_axis == Axis::Column ) {
        // 2. Set the child x position based on alignment style.
        switch ( parent_panel.children_horiz_align ) {
          case Align::Start: {
            transform.x = parent_panel.elem.transform.x;
          } break;
          case Align::SpaceOut: {
          } break;
        }

        // 3. Set the child y position based on alignment style.
        switch ( parent_panel.children_vert_align ) {
          case Align::Start: {
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

      if ( parent_panel.children_axis == Axis::Row ) {
        parent_panel.elem.transform.width = total_width;
        parent_panel.elem.transform.height = tallest_child;
      }
      else if ( parent_panel.children_axis == Axis::Column ) {
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
        case Type::StackPanel: {
          StackPanel &child_panel = Get<StackPanel>( child );

          child_panel.Draw();

          assert( Has<Panel>( child_panel.children[child_panel.curr_index] ) );

          RecursiveDraw(
            Get<Panel>( child_panel.children[child_panel.curr_index] )
          );

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
          Get<Panel>( child ), over_any_elem, mouseWentUp, mouseWentDown
        );
      }
      else if ( Has<StackPanel>( child ) ) {
        StackPanel &child_panel = Get<StackPanel>( child );

        assert( Has<Panel>( child_panel.children[child_panel.curr_index] ) );

        RecursiveInteractions(
          Get<Panel>( child_panel.children[child_panel.curr_index] ),
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
             child, inside, IsInteractive( child ), mouseWentUp, mouseWentDown
           ) ) {
        std::cout << "INTERACTION DETECTED!!!" << std::endl;

        if ( IsClickable( child ) ) {
          // action_lookup.at( GetId( child ) )();
          DoAction( child );
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
    }
    else if ( entity == context.hot ) {
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

  inline void ListenForSelect( entt::registry &game_reg, entt::entity entity ) {
    printf( "SelectListener?\n" );
    if ( game_reg.all_of<Province::Component>( entity ) ) {
      // Enabled the context panel
      auto &context = lookup.at( "settlement_context_panel" );
      RecursiveToggle( context, true );
      // ToggleElem( context, true );
      // UI::Panel context_panel = Get<UI::Panel>( context );

      // // Enable the tab group
      // entt::entity tab_grp = context_panel.children[0];
      // RecursiveToggle( tab_grp, true );

      // // Enable the content panel
      // entt::entity content = context_panel.children[1];
      // RecursiveToggle( content, true );
      // UI::StackPanel content_panel = Get<UI::StackPanel>( content );
    }
    else if ( game_reg.all_of<Actor::Component>( entity ) ) {
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

  inline bool MouseIsOverUI() {
    // This is almost sufficent, but we need to account for panels too
    // not just items that can be active?
    return context.active != entt::null || context.hot != entt::null;
  }

  inline void SetContextNull() {
    context.hot = entt::null;
    context.active = entt::null;
  }


};// namespace UI
