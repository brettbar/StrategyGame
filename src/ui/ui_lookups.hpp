/*
  This is where all UI actions will go. I centralize them here because that is easier to keep track of 
  than having the actions distributed to each ui elements file, importing stuff randomly all over.
  
  NOTE: When adding a new ui action, add both an action and clickable lambda to each of these looks up
*/

#pragma once

#include "../common.hpp"

#include "../save.hpp"
#include "../systems/actor_system.hpp"
#include "../systems/settlement_system.hpp"

namespace UI {

inline std::map<std::string, std::function<void()>> action_lookup = {
  {
    "actor_spawn_settlement_button",
    []() {
      printf( "Spawn Settlement clicked!!\n" );
      SettlementSystem::SpawnSettlement();
    },
  },
  {
    "settlement_context_tab_overview",
    []() { printf( "Settlement tab button pressed\n" ); },
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
    []() { printf( "Settlement tab button pressed\n" ); },
  },
  {
    "settlement_context_tab_garrison",
    []() { printf( "Settlement tab button pressed\n" ); },
  },
  {
    "modal_menu_load_game",
    []() {
      printf( "Load\n" );
      SaveSystem::Load();
    },
  },
  {
    "modal_menu_save_game",
    []() {
      printf( "Save\n" );
      SaveSystem::Save( Global::world );
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
      Global::program_mode = Global::ProgramMode::MainMenu;
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

inline std::map<std::string, std::function<bool()>> clickable_lookup = {
  {
    "actor_spawn_settlement_button",
    []() -> bool { return ActorSystem::ColonistCanPlaceSettlement(); },
  },
};

inline std::map<std::string, std::function<std::string()>> update_lookup = {
  {
    "settlement_name",
    []() -> std::string {
      if ( Global::world.all_of<Province::Component, Settlement::Component>(
             SelectionSystem::selected_entity
           ) ) {

        Province::Component &province = Global::world.get<Province::Component>(
          SelectionSystem::selected_entity
        );

        Settlement::Component &settlement =
          Global::world.get<Settlement::Component>(
            SelectionSystem::selected_entity
          );

        return settlement.name;
      }

      return "Uninhabited";
    },
  },
  {
    "settlement_population",
    []() -> std::string {
      if ( Global::world.all_of<Province::Component, Settlement::Component>(
             SelectionSystem::selected_entity
           ) ) {

        Province::Component &province = Global::world.get<Province::Component>(
          SelectionSystem::selected_entity
        );

        Settlement::Component &settlement =
          Global::world.get<Settlement::Component>(
            SelectionSystem::selected_entity
          );

        return std::to_string( settlement.population.current );
      }

      return "0";
    },
  },
  {
    "settlement_development",
    []() -> std::string {
      if ( Global::world.all_of<Province::Component, Settlement::Component>(
             SelectionSystem::selected_entity
           ) ) {

        Province::Component &province = Global::world.get<Province::Component>(
          SelectionSystem::selected_entity
        );

        Settlement::Component &settlement =
          Global::world.get<Settlement::Component>(
            SelectionSystem::selected_entity
          );

        return Settlement::development.at( settlement.development );
      }

      return "Uninhabited";
    },
  },
};

};// namespace UI
