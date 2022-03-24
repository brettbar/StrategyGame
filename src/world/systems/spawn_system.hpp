#pragma once

#include "../../common.hpp"
#include "event_system.hpp"
#include "../components/animated.hpp"
#include "../components/selected.hpp"
#include "../components/sight.hpp"
#include "../components/unit.hpp"
#include "../entities/archetypes.hpp"

#include "../../resource.hpp"
#include "../player_system.hpp"

namespace SpawnSystem {
inline void DeleteSelected( entt::registry & );
Texture2D DetermineTexture( Faction, TextureCache & );

struct SpawnListener : EventSystem::Listener {
  inline void Receive() override {
    if ( currState == nullptr || currReg == nullptr )
      return;

    printf( "SpawnSystem got an event!\n" );
    DeleteSelected( *currReg );
  }

  inline void Listen() {
    EventSystem::dispatcher.sink<Event::SpawnEvent>()
      .connect<&SpawnListener::Receive>( this );
  }
};

inline SpawnListener listener;

inline void Init() { listener.Listen(); }

inline void Update( State &state, entt::registry &reg ) {
  listener.Update( state, reg );
}

inline void CreateNew(
  entt::registry &reg,
  TextureCache &cache,
  Vector2 clickPos,
  std::shared_ptr<PlayerSystem> currPlayer ) {
  std::unique_ptr<Vector2> spawn = DetermineTilePos( clickPos );
  assert( spawn != nullptr );

  entt::entity entity = reg.create();

  Texture tex =
    cache.handle( currPlayer->textureMap.at( "Villager" ) )->texture;
  Archetypes::Actor actor = Archetypes::Actor( tex, *spawn );

  reg.emplace<Unit::Component>( entity, actor.unit );
  reg.emplace<Animated::Component>( entity, actor.animated );
  reg.emplace<Sight::Component>( entity, actor.sight );
}

inline void DeleteSelected( entt::registry &reg ) {
  auto selectedView = reg.view<Selected::Component, Unit::Component>();
  auto selectedEntity = selectedView.front();

  if ( selectedEntity == entt::null ) {
    printf( "No selected entity, cancelling\n" );
    return;
  }

  reg.destroy( selectedEntity );
}

};// namespace SpawnSystem
