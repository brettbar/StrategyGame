#pragma once

#include "../archetypes/archetypes.hpp"
#include "../common.hpp"
#include "../components/actor.hpp"
#include "../components/animated.hpp"
#include "../components/event.hpp"
#include "../components/selected.hpp"
#include "../components/sight.hpp"
#include "../components/unit.hpp"
#include "../events.hpp"
#include "../global.hpp"

#include "player_system.hpp"
#include "selection_system.hpp"

namespace SpawnSystem {
inline void CreateNew( TextureCache &, Vector2, std::shared_ptr<PlayerSystem> );
inline void DeleteSelected();
Texture2D DetermineTexture( Faction, TextureCache & );

// struct SpawnListener : Events::Listener {
//   inline void Receive() override {
//     if ( currState == nullptr )
//       return;

//     printf( "SpawnSystem got an event!\n" );
//     DeleteSelected();
//   }

//   inline void Listen() {
//     Events::dispatcher.sink<Event::SpawnEvent>()
//       .connect<&SpawnListener::Receive>( this );
//   }
// };

// inline SpawnListener listener;

inline void Init() {
  // listener.Listen();
}

inline void Update( State &state ) {
  // listener.Update( state );
}

inline void CreateNew(
  TextureCache &cache,
  Vector2 clickPos,
  std::shared_ptr<PlayerSystem> currPlayer ) {
  std::unique_ptr<Vector2> spawn = DetermineTilePos( clickPos );
  assert( spawn != nullptr );

  entt::entity entity = Global::registry.create();

  entt::resource_cache<TextureResource> temp{};

  Texture2D tex = cache[currPlayer->textureMap.at( "Villager" )]->texture;

  Archetypes::Character character = Archetypes::Character( tex, *spawn );

  Global::registry.emplace<Actor::Component>(
    entity,
    "Marcus Priscus",
    Actor::Type::Colonist );
  Global::registry.emplace<Unit::Component>( entity, character.unit );
  Global::registry.emplace<Animated::Component>( entity, character.animated );
  Global::registry.emplace<Sight::Component>( entity, character.sight );
}

inline void DeleteSelected() {
  auto selectedView =
    Global::registry.view<Selected::Component, Unit::Component>();
  auto selectedEntity = selectedView.front();

  if ( selectedEntity == entt::null ) {
    printf( "No selected entity, cancelling\n" );
    return;
  }

  Global::registry.destroy( selectedEntity );
}

};// namespace SpawnSystem
