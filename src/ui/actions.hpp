#pragma once

#include "../global.hpp"

#include "../events.hpp"
#include "../systems/settlement_system.hpp"

#include "components/panel.hpp"
#include "components/texture_elements.hpp"
#include "ui_lookup.hpp"
#include "ui_utils.hpp"

namespace UI {

inline std::map<std::string, std::function<void()>> action_lookup = {
  {
    "actor_spawn_settlement_button",
    []() {
      printf( "Spawn Settlement clicked!!\n" );
      SettlementSystem::SpawnSettlement();

      // TODO handle this in main
      // Events::event_emitter.publish( Events::UIEvent{
      //   "actor_spawn_settlement_button",
      // } );
    },
  },
  {
    "settlement_context_tab_overview",
    []() {
      printf( "Settlement tab button pressed\n" );
      entt::entity content = lookup.at( "settlement_context_content" );
      StackPanel &sp = Get<StackPanel>( content );
      SwitchChild( sp, 0 );
    },
  },
  {
    "settlement_context_tab_population",
    []() { printf( "Settlement tab button pressed\n" ); },
  },
  {
    "settlement_context_tab_resources",
    []() { printf( "Settlement tab button pressed\n" ); },
  },
  {
    "settlement_context_tab_culture",
    []() { printf( "Settlement tab button pressed\n" ); },
  },
  {
    "settlement_context_tab_religion",
    []() { printf( "Settlement tab button pressed\n" ); },
  },
  {
    "settlement_context_tab_construction",
    []() {
      printf( "Settlement tab button pressed\n" );
      entt::entity content = lookup.at( "settlement_context_content" );
      StackPanel &sp = Get<StackPanel>( content );
      SwitchChild( sp, 1 );
    },
  },
  {
    "settlement_context_tab_garrison",
    []() { printf( "Settlement tab button pressed\n" ); },
  },
  // Main Menu
  {
    "main_menu_resume_game",
    []() {
      printf( "Resume\n" );
      Events::event_emitter.publish( Events::UIEvent{
        "main_menu_resume_game",
      } );
    },
  },
  {
    "main_menu_start_game",
    []() {
      printf( "Start\n" );
      Events::event_emitter.publish( Events::UIEvent{
        "main_menu_start_game",
      } );
    },
  },
  {
    "main_menu_load_game",
    []() {
      printf( "Load\n" );
      Events::event_emitter.publish( Events::UIEvent{
        "main_menu_load_game",
      } );
    },
  },
  {
    "main_menu_settings",
    []() { printf( "Settings\n" ); },
  },
  {
    "main_menu_exit_game",
    []() {
      printf( "ExitGame\n" );
      Events::event_emitter.publish( Events::UIEvent{
        "main_menu_exit_game",
      } );
    },
  },
  // Modal Menu
  {
    "modal_menu_load_game",
    []() {
      printf( "Load\n" );
      Events::event_emitter.publish( Events::UIEvent{
        "modal_menu_load_game",
      } );
    },
  },
  {
    "modal_menu_save_game",
    []() {
      printf( "Save\n" );
      Events::event_emitter.publish( Events::UIEvent{
        "modal_menu_save_game",
      } );
    },
  },
  {
    "modal_menu_settings",
    []() { printf( "Settings\n" ); },
  },
  {
    "modal_menu_save_exit_main",
    []() { printf( "SaveExitMain\n" ); },
  },
  {
    "modal_menu_exit_main",
    []() {
      printf( "ExitMain\n" );
      Events::event_emitter.publish( Events::UIEvent{ "modal_menu_exit_main" }
      );
    },
  },
  {
    "modal_menu_exit_game",
    []() {
      printf( "ExitGame\n" );
      CloseWindow();
    },
  },
};

};
