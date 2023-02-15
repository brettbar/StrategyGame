#pragma once

#include "pages/campaign_ui.hpp"
#include "pages/faction_select_menu.hpp"
#include "pages/lobby_browser.hpp"
#include "pages/lobby_ui.hpp"
#include "pages/main_menu_ui.hpp"
#include "pages/modal_menu_ui.hpp"
#include "pages/singleplayer_lobby.hpp"
#include "ui_manager.hpp"

namespace UI {

  namespace System {

    void EnableContent();
    void DisableCurrentContent();
    void RecursiveLayout( ptr<Panel> );

    void Draw();

    inline void Init() {
      Manager()->pages = {
        CreateMainMenuUI(),
        CreateFactionSelectMenuUI(),
        CreateSinglePlayerLobby(),
        CreateModalMenuUI(),
        CreateCampaignUI(),
        // CreateLobbyBrowser(),
        // CreateLobbyUI(),
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
      for ( ptr<Element> base: Manager()->ActivePage() ) {
        // RecursiveToggle( base, true );
        base->Enable();
      }
    }

    inline void DisableCurrentContent() {
      Manager()->SetContextNull();

      for ( ptr<Element> base: Manager()->ActivePage() ) {
        // RecursiveToggle( base, false );
        base->Disable();
      }
    }

    inline void UpdateOnFrame() {
      vec2 mousePos = GetMousePosition();
      bool mouseWentUp = IsMouseButtonReleased( 0 );
      bool mouseWentDown = IsMouseButtonPressed( 0 );
      bool mouseHeldDown = IsMouseButtonDown( 0 );
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

      Messages::dispatcher.update();

      Manager()->over_any_elem = false;

      for ( ptr<Element> base: Manager()->ActivePage() ) {
        ptr<Panel> panel = Get<Panel>( base );
        RecursiveLayout( panel );

        panel->Interact( mouseWentUp, mouseWentDown );
      }

      if ( !Manager()->over_any_elem )
        Manager()->SetContextNull();
    }

    inline void Draw() {
      for ( ptr<Element> base: Manager()->ActivePage() ) {
        base->Draw();
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
    inline void RecursiveLayout( ptr<Panel> parent_panel ) {
      if ( !parent_panel->IsEnabled() )
        return;

      f32 total_height = 0;
      f32 total_width = 0;
      f32 tallest_child = 0;
      f32 widest_child = 0;
      f32 end_of_last_x = parent_panel->transform.x;
      f32 end_of_last_y = parent_panel->transform.y;

      parent_panel->Update();

      for ( ptr<Element> child: parent_panel->children ) {
        // TODO(??) not sure if this is right
        if ( !child )
          continue;


        if ( Has<Panel>( child ) ) {
          ptr<Panel> child_panel = Get<Panel>( child );
          RecursiveLayout( child_panel );
        }
        else if ( Has<StackPanel>( child ) ) {
          ptr<StackPanel> child_panel = Get<StackPanel>( child );

          ptr<Element> single_child =
            child_panel->children[child_panel->curr_index];

          assert( Has<Panel>( single_child ) );
          ptr<Panel> single_child_panel = Get<Panel>( single_child );
          single_child_panel->transform.x = child_panel->transform.x;
          single_child_panel->transform.y = child_panel->transform.y;

          RecursiveLayout( Get<Panel>( single_child ) );
        }
        else {
          child->Resize();
        }

        if ( parent_panel->children_axis == Axis::Row ) {
          // 2. Set the child x position based on alignment style.
          switch ( parent_panel->children_horiz_align ) {
            case Align::Start: {
              child->transform.x = end_of_last_x + child->margins.left;
              end_of_last_x = child->transform.x + child->transform.width +
                              child->margins.right;
            } break;
            case Align::SpaceOut: {
            } break;
          }

          // 3. Set the child y position based on alignment style.
          switch ( parent_panel->children_vert_align ) {
            case Align::Start: {
              child->transform.y = parent_panel->transform.y;
            } break;
          }
        }
        else if ( parent_panel->children_axis == Axis::Column ) {
          // 2. Set the child x position based on alignment style.
          switch ( parent_panel->children_horiz_align ) {
            case Align::Start: {
              child->transform.x = parent_panel->transform.x;
            } break;
            case Align::SpaceOut: {
            } break;
          }

          // 3. Set the child y position based on alignment style.
          switch ( parent_panel->children_vert_align ) {
            case Align::Start: {
              child->transform.y = end_of_last_y;
              // + margins.top;
              end_of_last_y = child->transform.y + child->transform.height;
              // + margins.bottom;
            } break;
          }
        }
      }

      if ( !parent_panel->abs_size ) {
        for ( ptr<Element> child: parent_panel->children ) {
          // TODO(??) not sure if this is right
          if ( !child )
            continue;

          total_width += child->transform.width;
          total_height += child->transform.height;

          if ( child->transform.width > widest_child )
            widest_child = child->transform.width;

          if ( child->transform.height > tallest_child )
            tallest_child = child->transform.height;
        }

        if ( parent_panel->children_axis == Axis::Row ) {
          parent_panel->transform.width = total_width;
          parent_panel->transform.height = tallest_child;
        }
        else if ( parent_panel->children_axis == Axis::Column ) {
          parent_panel->transform.width = widest_child;
          parent_panel->transform.height = total_height;
        }
      }
    }


    // inline void RecursiveInteractions(
    //   ptr<Panel> panel,
    //   bool mouseWentUp,
    //   bool mouseWentDown
    // ) {

    //   if ( !panel->IsEnabled() )
    //     return;

    //   for ( ptr<Element> child: panel->children ) {
    //     if ( !child )
    //       continue;
    //     if ( !child->IsEnabled() )
    //       continue;

    //     // UpdateElem( child );
    //     child->Interact( mouseWentUp, mouseWentDown );
    //   }
    // }


  };// namespace System

};// namespace UI
