#pragma once


#include "../../shared/common.hpp"
#include "../../shared/global.hpp"

#include "../../network/client.hpp"
#include "../../network/host.hpp"

#include "../managers/faction_manager.hpp"

#include "../components/ai_component.hpp"
#include "../components/faction_component.hpp"
#include "../components/player_component.hpp"

#include "ai_system.hpp"


namespace Player {

  struct System {
    static void create_players_for_sp(str player_faction) {
      {
        auto player = Global::world.create();
        Global::world.emplace<Player::Component>(
          player, player, "player_0", true
        );
        Global::world.emplace<Player::LocalTag>(player);
        Global::world.emplace<Faction::Component>(
          player, Faction::Manager::Get()->factions.at(player_faction)
        );
      }


      {
        auto ai_player = Global::world.create();
        Global::world.emplace<Player::Component>(
          ai_player, ai_player, "player_1", true
        );
        Global::world.emplace<Faction::Component>(
          ai_player, Faction::Manager::Get()->factions.at("greeks")
        );

        AI::System::Create(ai_player);
      }

      // {
      //   auto ai_player = Global::world.create();
      //   Global::world.emplace<Player::Component>(
      //     ai_player, ai_player, "player_2", true
      //   );
      //   Global::world.emplace<Faction::Component>(
      //     ai_player, Faction::Manager::Get()->factions.at("celts")
      //   );
      //
      //   AI::System::Create(ai_player);
      // }
      //
      //
      // {
      //   auto ai_player = Global::world.create();
      //   Global::world.emplace<Player::Component>(
      //     ai_player, ai_player, "player_3", true
      //   );
      //   Global::world.emplace<Faction::Component>(
      //     ai_player, Faction::Manager::Get()->factions.at("carthaginians")
      //   );
      //
      //   AI::System::Create(ai_player);
      // }
    }

    static void HostStartMultiplayer() {
      for (auto client: Network::Host()->_clients) {
        if (!client.peer_data.active)
          continue;

        auto player = Global::world.create();
        Global::world.emplace<Player::Component>(
          player, player, client.peer_data.player_id, true
        );

        if (client.peer_data.player_id == "player_0")
          Global::world.emplace<Player::LocalTag>(player);
        else
          Global::world.emplace<Player::RemoteTag>(player);

        Global::world.emplace<Faction::Component>(
          player, Faction::Manager::Get()->factions.at(client.peer_data.faction)
        );
      }

      for (auto player_e:
           Global::world.view<Player::Component, Faction::Component>()) {
        Player::Component player =
          Global::world.get<Player::Component>(player_e);
        Faction::Component faction =
          Global::world.get<Faction::Component>(player_e);

        std::cout << "Initialized Player: " << player.player_id << '\n';
        std::cout << "Faction: " << faction.id << '\n';
        if (Global::world.any_of<Player::LocalTag>(player_e)) {
          std::cout << "with LocalTag!" << '\n';
        }
        if (Global::world.any_of<Player::RemoteTag>(player_e)) {
          std::cout << "with RemoteTag!" << '\n';
        }
      }
    }

    static void ClientStartMultiplayer() {
      for (auto peer: Network::Client()->_peers) {
        if (!peer.active)
          continue;

        auto player = Global::world.create();
        Global::world.emplace<Player::Component>(
          player, player, peer.player_id, true
        );

        if (Network::Client()->_local_player_id == peer.player_id)
          Global::world.emplace<Player::LocalTag>(player);
        else
          Global::world.emplace<Player::RemoteTag>(player);

        Global::world.emplace<Faction::Component>(
          player, Faction::Manager::Get()->factions.at(peer.faction)
        );
      }

      auto view = Global::world.view<Player::Component, Faction::Component>();
      for (auto player_e: view) {

        auto player_c = view.get<Player::Component>(player_e);
        auto faction_c = view.get<Faction::Component>(player_e);

        std::cout << "Initialized Player: " << player_c.player_id << '\n';
        std::cout << "Faction: " << faction_c.id << '\n';
        if (Global::world.any_of<Player::LocalTag>(player_e)) {
          std::cout << "with LocalTag!" << '\n';
        }
        if (Global::world.any_of<Player::RemoteTag>(player_e)) {
          std::cout << "with RemoteTag!" << '\n';
        }
      }
    }


    static void Update(view<Player::Component> players) {
      // Change to exclude AI
      // for ( auto entity: players ) {
      //   Player::Component &player =
      //     Global::world.get<Player::Component>( entity );

      //   if ( player.is_human ) {
      //     HumanUpdate( player );
      //   }
      // }

      auto ai_view = Global::world.view<Player::Component, AI::Component>();
      for (auto ai_player: ai_view) {
        auto [player_c, ai_c] =
          ai_view.get<Player::Component, AI::Component>(ai_player);

        if (Global::state.timeScale <= 0) {
        } else {
          AI::System::update(ai_player, player_c, ai_c);
        }
      }
    }

    static entt::entity GetEntityOfPlayer(str player_id) {
      auto v = Global::world.view<Player::Component>();

      for (auto p: v) {
        Player::Component pc = v.get<Player::Component>(p);

        if (pc.player_id == player_id) {
          return p;
        }
      }

      // Should never get here
      return entt::null;
    }
  };
};// namespace Player
