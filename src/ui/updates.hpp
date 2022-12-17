#pragma once

#include "../common.hpp"
#include "../global.hpp"

#include "../events.hpp"

#include "../components/province.hpp"
#include "../components/settlement.hpp"
#include "../systems/selection_system.hpp"

namespace UI {

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
