#pragma once

#include "../components/ai.hpp"
#include "../global.hpp"

namespace AISystem {

inline void Init();
inline void Update();
inline void CreateAIPlayers();

inline void Init() { CreateAIPlayers(); }

inline void Update() {
  for ( auto entity: Global::registry.view<AI::Component>() ) {
  }
}

inline void CreateAIPlayers() {
  entt::entity new_player = Global::registry.create();
  Global::registry.emplace<AI::Component>( new_player, 1 );
}


};// namespace AISystem
