//
// Created by brett on 1/30/2022.
//
#pragma once

#include "../../shared/global.hpp"

#include "../entities/actor_entity.hpp"

#include "../managers/map_manager.hpp"

#include "../components/actor_component.hpp"
#include "../components/player_component.hpp"

#include "../systems/movement_system.hpp"
#include "../systems/province_system.hpp"
#include "../systems/selection_system.hpp"


#include <filesystem>
#include <fstream>


namespace Actor {

  class System {
public:
    static void spawn_colonist( entt::entity owner, Vector2 clickPos ) {
      std::unique_ptr<Vector2> spawn = DetermineTilePos( clickPos );
      assert( spawn != nullptr );

      create_colonist( owner, *spawn );
    }

    static void spawn_army( entt::entity owner, Vector2 clickPos ) {
      std::unique_ptr<Vector2> spawn = DetermineTilePos( clickPos );
      assert( spawn != nullptr );

      create_army( owner, *spawn );
    }


    static void Init() {

      auto players = Global::world.view<Player::Component>();

      for ( auto player_e: players ) {
        auto player = players.get<Player::Component>( player_e );

        if ( player.player_id == "player_0" ) {
          spawn_colonist( player.id, { 85 * TILE_WIDTH, 65 * TILE_HEIGHT } );
        }

        if ( player.player_id == "player_1" ) {
          spawn_colonist( player.id, { 80 * TILE_WIDTH, 65 * TILE_HEIGHT } );
        }

        if ( player.player_id == "player_2" ) {
          spawn_colonist( player.id, { 85 * TILE_WIDTH, 70 * TILE_HEIGHT } );
        }

        if ( player.player_id == "player_3" ) {
          spawn_colonist( player.id, { 80 * TILE_WIDTH, 70 * TILE_HEIGHT } );
        }
      }
    }

    static entt::entity get_colonist_of_player( entt::entity owner ) {
      auto actors = Global::world.view<Actor::Component>();

      for ( auto actor_e: actors ) {
        Actor::Component actor = actors.get<Actor::Component>( actor_e );

        if ( actor.data.type == Actor::Type::Colonist && actor.owner == owner ) {
          return actor_e;
        }
      }

      return entt::null;
    }


    static bool colonist_can_claim_province( entt::entity colonist ) {
      if ( MovementSystem::ActorIsMoving( colonist ) )
        return false;

      Actor::Component actor = Global::world.get<Actor::Component>( colonist );

      i32 closest_tile = DetermineTileIdFromPosition( actor.position );
      // 2. if they aren't in a tile, bail
      if ( closest_tile == -1 )
        return false;

      for ( auto entity: Global::world.view<Province::Component>() ) {
        auto &prov = Global::world.get<Province::Component>( entity );

        // 3. if the closest tile is unowned and of a valid biome
        if ( 
          prov.tile->id == closest_tile && 
          prov.tile->owner == entt::null && 
          Map::Manager()->biome_inhabitable(prov.tile->biome)
      ) {
          return true;
        }
      }

      // Otherwise, default to false
      return false;
    }

    static void colonist_build_settlement( entt::entity colonist ) {
      Actor::Component actor = Global::world.get<Actor::Component>( colonist );

      i32 closest_tile = DetermineTileIdFromPosition( actor.position );
      // 2. if they aren't in a tile, bail
      if ( closest_tile == -1 )
        return;
    }

    static void colonist_claim_province( entt::entity colonist ) {
      Actor::Component actor = Global::world.get<Actor::Component>( colonist );

      i32 closest_tile = DetermineTileIdFromPosition( actor.position );
      // 2. if they aren't in a tile, bail
      if ( closest_tile == -1 )
        return;

      ProvinceSystem::colonist_claim_province( colonist );
    }


    static bool colonist_can_place_settlement( entt::entity colonist ) {
      // 1. if the colonist is moving, bail
      if ( MovementSystem::ActorIsMoving( colonist ) )
        return false;

      Actor::Component actor = Global::world.get<Actor::Component>( colonist );

      i32 closest_tile = DetermineTileIdFromPosition( actor.position );

      // 2. if they aren't in a tile, bail
      if ( closest_tile == -1 )
        return false;

      for ( auto entity: Global::world.view<Province::Component>() ) {
        auto &prov = Global::world.get<Province::Component>( entity );

        // !3. if the closest tile is owned by our faction, and the tile doesn't already have a settlement
        if ( prov.tile->id == closest_tile && prov.tile->owner == actor.owner && !Global::world.any_of<Settlement::Component>( entity ) ) {
          return true;
        }
      }

      // Otherwise, default to false
      return false;
    }

    void DeleteSelected() {
      auto selectedView =
        Global::world.view<Selected::Component, Actor::Component>();
      auto selectedEntity = selectedView.front();

      if ( selectedEntity == entt::null ) {
        printf( "No selected entity, cancelling\n" );
        return;
      }

      Global::world.destroy( selectedEntity );
    }

private:
    // A settlement can be placed when
    // 0. A colonist is selected
    // 1. The colonist is not moving
    // 2. The colonist is in a province owned by their faction
    // 3. The province does not already contain a settlement
    bool selected_colonist_can_place_settlement() {
      entt::entity selected_entity = Selection::GetSelectedEntity();
      // 0. if the colonist isnt selected, bail
      if ( selected_entity == entt::null || !Global::world.all_of<Actor::Component>( selected_entity ) )
        return false;

      return colonist_can_place_settlement( selected_entity );
    }
  };

};// namespace Actor
