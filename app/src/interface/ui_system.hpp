#pragma once

#include "pages/campaign_ui.hpp"
#include "pages/faction_select_menu.hpp"
#include "pages/lobby_browser.hpp"
#include "pages/lobby_ui.hpp"
#include "pages/main_menu_ui.hpp"
#include "pages/modal_menu_ui.hpp"
#include "ui_manager.hpp"

namespace UI {

  namespace System {

    void EnableContent();
    void DisableCurrentContent();
    void RecursiveLayout( Panel & );
    void RecursiveInteractions( Panel &, bool &, bool, bool );

    void RecursiveDraw( Panel & );
    void Draw();

    inline void Init() {
      Manager()->pages = {
        CreateMainMenuUI(),
        CreateFactionSelectMenuUI(),
        CreateModalMenuUI(),
        CreateCampaignUI(),
        CreateLobbyBrowser(),
        CreateLobbyUI(),
      };
      Manager()->SetScene( MainMenu );
      EnableContent();
    }

    inline void SwitchPage( PageType page ) {
      DisableCurrentContent();
      Manager()->SetScene( page );
      EnableContent();
    }

    inline void EnableCampaignUI() {
      DisableCurrentContent();
      Manager()->SetScene( Campaign );
    }

    inline void EnableContent() {
      for ( entt::entity base: Manager()->ActivePage() ) {
        RecursiveToggle( base, true );
      }
    }

    inline void DisableCurrentContent() {
      Manager()->SetContextNull();
      for ( entt::entity base: Manager()->ActivePage() ) {
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

      for ( entt::entity base: Manager()->ActivePage() ) {
        Panel &panel = Get<Panel>( base );
        RecursiveLayout( panel );
        RecursiveInteractions(
          panel, over_any_elem, mouseWentUp, mouseWentDown
        );
      }
    }

    inline void Draw() {
      for ( entt::entity base: Manager()->ActivePage() ) {
        Panel &panel = Get<Panel>( base );
        RecursiveDraw( panel );
      }

      DrawRectangle( GetScreenWidth() - 120, 2, 100, 24.0f, BLACK );
      DrawFPS( GetScreenWidth() - 100, 2 );

      Manager()->DrawManagerDebugInfo();

      DrawRectangle( GetScreenWidth() - 600, 252, 200, 24.0f, BLACK );
      DrawText(
        std::to_string( Global::state.timeScale ).c_str(),
        GetScreenWidth() - 600,
        252,
        24.0f,
        WHITE
      );
    }

    // TODO this and the panel one are likely causing
    // That visual bug on the first frame.
    // We make a call to draw before the panel is sized if its
    // relative
    inline void RecursiveLayout( Panel &parent_panel ) {
      if ( !parent_panel.elem.enabled )
        return;

      f32 total_height = 0;
      f32 total_width = 0;
      f32 tallest_child = 0;
      f32 widest_child = 0;
      f32 end_of_last_x = parent_panel.elem.transform.x;
      f32 end_of_last_y = parent_panel.elem.transform.y;

      parent_panel.Update();

      for ( entt::entity child: parent_panel.children ) {

        if ( Has<Panel>( child ) ) {
          Panel &child_panel = Get<Panel>( child );
          RecursiveLayout( child_panel );
        }
        else if ( Has<StackPanel>( child ) ) {
          StackPanel &child_panel = Get<StackPanel>( child );

          entt::entity single_child =
            child_panel.children[child_panel.curr_index];

          assert( Has<Panel>( single_child ) );
          Panel &single_child_panel = Get<Panel>( single_child );
          single_child_panel.elem.transform.x = child_panel.elem.transform.x;
          single_child_panel.elem.transform.y = child_panel.elem.transform.y;

          RecursiveLayout( Get<Panel>( single_child ) );
        }
        else if ( Has<TextLabel>( child ) ) {
          Get<TextLabel>( child ).Resize();
        }
        else if ( Has<TextButton>( child ) ) {
          Get<TextButton>( child ).label.Resize();
        }
        else if ( Has<TextureLabel>( child ) ) {
          Get<TextureLabel>( child ).Resize();
        }
        else if ( Has<TextureButton>( child ) ) {
          Get<TextureButton>( child ).label.Resize();
        }

        // TODO ^ these could probably be consolidated using a template
        rect &transform = GetTransform( child );
        Margins &margin = GetMargins( child );

        if ( parent_panel.children_axis == Axis::Row ) {
          // 2. Set the child x position based on alignment style.
          switch ( parent_panel.children_horiz_align ) {
            case Align::Start: {
              transform.x = end_of_last_x + margin.left;
              end_of_last_x = transform.x + transform.width + margin.right;
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
        if ( Has<Panel>( child ) ) {
          RecursiveDraw( Get<Panel>( child ) );
        }
        else if ( Has<StackPanel>( child ) ) {
          StackPanel &child_panel = Get<StackPanel>( child );

          child_panel.Draw();

          assert( Has<Panel>( child_panel.children[child_panel.curr_index] ) );

          RecursiveDraw(
            Get<Panel>( child_panel.children[child_panel.curr_index] )
          );
        }
        else if ( Has<TextLabel>( child ) ) {
          Get<TextLabel>( child ).Draw();
        }
        else if ( Has<TextButton>( child ) ) {
          Get<TextButton>( child ).Draw();
        }
        else if ( Has<TextureLabel>( child ) ) {
          Get<TextureLabel>( child ).Draw();
        }
        else if ( Has<TextureButton>( child ) ) {
          Get<TextureButton>( child ).Draw();
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

        if ( Manager()->DoInteraction(
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
        Manager()->SetContextNull();
      }
    }

    inline void
    ListenForSelect( entt::registry &game_reg, entt::entity entity ) {
      printf( "SelectListener?\n" );
      if ( game_reg.all_of<Province::Component>( entity ) ) {
        // Enabled the context panel
        auto &context = Manager()->lookup.at( "settlement_context_panel" );
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

        auto context_panel = Manager()->lookup.at( "actor_context_panel" );
        RecursiveToggle( context_panel, true );
      }
    }

    inline void ListenForDeselect() {
      printf( "DeSelectListener?\n" );
      auto context_panel = Manager()->lookup.at( "settlement_context_panel" );
      RecursiveToggle( context_panel, false );
      context_panel = Manager()->lookup.at( "actor_context_panel" );
      RecursiveToggle( context_panel, false );
      Manager()->SetContextNull();
    }

  };// namespace System

};// namespace UI
