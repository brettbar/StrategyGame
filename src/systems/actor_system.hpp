//
// Created by brett on 1/30/2022.
//
#include "../components/actor.hpp"
#include "../global.hpp"
#include "selection_system.hpp"

namespace ActorSystem {
inline bool ColonistSelected();

inline bool ColonistSelected() {
  if ( Global::registry.any_of<Actor::Component>(
         SelectionSystem::selected_entity ) ) {
    return Global::registry
             .get<Actor::Component>( SelectionSystem::selected_entity )
             .type == Actor::Type::Colonist;
  }
  return false;
}
};// namespace ActorSystem
