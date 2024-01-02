#pragma once


#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../components/ai.hpp"
#include "../components/player.hpp"

#include "../components/faction.hpp"
#include "../systems/faction_system.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"

namespace PlayerSystem {
  inline void HumanUpdate( Player::Component & );
  inline void AIUpdate( AI::Component &, Player::Component & );

  inline void StartSingleplayer( str player_faction ) {
    {
      auto player = Global::world.create();
      Global::world.emplace<Player::Component>(
        player, player, "player_0", true
      );
      Global::world.emplace<Player::LocalTag>( player );
      Global::world.emplace<Faction::Component>(
        player, FactionSystem::factions.at( player_faction )
      );
    }


    {
      auto ai_player = Global::world.create();
      Global::world.emplace<Player::Component>(
        ai_player, ai_player, "player_1", true
      );
      Global::world.emplace<Faction::Component>(
        ai_player, FactionSystem::factions.at( "greeks" )
      );
    }
  }

  inline void HostStartMultiplayer() {
    for ( auto client: Network::Host()->_clients ) {
      if ( !client.peer_data.active )
        continue;

      auto player = Global::world.create();
      Global::world.emplace<Player::Component>(
        player, player, client.peer_data.player_id, true
      );

      if ( client.peer_data.player_id == "player_0" )
        Global::world.emplace<Player::LocalTag>( player );
      else
        Global::world.emplace<Player::RemoteTag>( player );

      Global::world.emplace<Faction::Component>(
        player, FactionSystem::factions.at( client.peer_data.faction )
      );
    }

    for ( auto player_e:
          Global::world.view<Player::Component, Faction::Component>() ) {
      Player::Component player =
        Global::world.get<Player::Component>( player_e );
      Faction::Component faction =
        Global::world.get<Faction::Component>( player_e );

      std::cout << "Initialized Player: " << player.player_id << '\n';
      std::cout << "Faction: " << faction.id << '\n';
      if ( Global::world.any_of<Player::LocalTag>( player_e ) ) {
        std::cout << "with LocalTag!" << '\n';
      }
      if ( Global::world.any_of<Player::RemoteTag>( player_e ) ) {
        std::cout << "with RemoteTag!" << '\n';
      }
    }
  }

  inline void ClientStartMultiplayer() {
    for ( auto peer: Network::Client()->_peers ) {
      if ( !peer.active )
        continue;

      auto player = Global::world.create();
      Global::world.emplace<Player::Component>(
        player, player, peer.player_id, true
      );

      if ( Network::Client()->_local_player_id == peer.player_id )
        Global::world.emplace<Player::LocalTag>( player );
      else
        Global::world.emplace<Player::RemoteTag>( player );

      Global::world.emplace<Faction::Component>(
        player, FactionSystem::factions.at( peer.faction )
      );
    }

    for ( auto player_e:
          Global::world.view<Player::Component, Faction::Component>() ) {
      Player::Component player =
        Global::world.get<Player::Component>( player_e );
      Faction::Component faction =
        Global::world.get<Faction::Component>( player_e );

      std::cout << "Initialized Player: " << player.player_id << '\n';
      std::cout << "Faction: " << faction.id << '\n';
      if ( Global::world.any_of<Player::LocalTag>( player_e ) ) {
        std::cout << "with LocalTag!" << '\n';
      }
      if ( Global::world.any_of<Player::RemoteTag>( player_e ) ) {
        std::cout << "with RemoteTag!" << '\n';
      }
    }
  }


  inline void Update( view<Player::Component> players ) {
    // Change to exclude AI
    for ( auto entity: players ) {
      Player::Component &player =
        Global::world.get<Player::Component>( entity );

      if ( player.is_human ) {
        HumanUpdate( player );
      }
    }

    for ( auto entity:
          Global::world.view<Player::Component, AI::Component>() ) {
      Player::Component &player =
        Global::world.get<Player::Component>( entity );
      AI::Component &ai = Global::world.get<AI::Component>( entity );

      AIUpdate( ai, player );
    }
  }

  inline void HumanUpdate( Player::Component &player ) {}

  inline void AIUpdate( AI::Component &ai, Player::Component &player ) {
    switch ( ai.current_goal ) {
      case AI::Goal::DevelopSettlements: {
        if ( !ai.has_colonist ) {
          // ActorSystem::SpawnColonist( player.id );
          ai.has_colonist = true;
        } else if ( ai.has_colonist && !ai.has_settlement ) {
        }
      } break;
    }
  }


};// namespace PlayerSystem
