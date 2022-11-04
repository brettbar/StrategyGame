// TODO this shouldnt be its own system. Move the spawns to their respective systems.

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
inline void SpawnColonist( Vector2 );
inline void DeleteSelected();

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

inline void SpawnColonist( Vector2 clickPos ) {
  std::unique_ptr<Vector2> spawn = DetermineTilePos( clickPos );
  assert( spawn != nullptr );

  entt::entity entity = Global::world.create();

  entt::resource_cache<TextureResource> temp{};

  // Texture2D tex = cache[currPlayer->textureMap.at( "Villager" )]->texture;

  Texture2D tex =
    Global::texture_cache[hstr{ "romanVillagerTexture" }]->texture;

  Archetypes::Character character = Archetypes::Character( tex, *spawn );

  Global::world.emplace<Actor::Component>(
    entity,
    "Marcus Priscus",
    Actor::Type::Colonist );
  Global::world.emplace<Unit::Component>( entity, character.unit );
  Global::world.emplace<Animated::Component>( entity, character.animated );
  Global::world.emplace<Sight::Component>( entity, character.sight );
}

inline void DeleteSelected() {
  auto selectedView =
    Global::world.view<Selected::Component, Unit::Component>();
  auto selectedEntity = selectedView.front();

  if ( selectedEntity == entt::null ) {
    printf( "No selected entity, cancelling\n" );
    return;
  }

  Global::world.destroy( selectedEntity );
}

};// namespace SpawnSystem
