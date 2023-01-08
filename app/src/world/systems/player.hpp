#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../components/ai.hpp"

namespace Player {

  struct Component {
    entt::entity id;
    bool is_human;
    std::string faction;

    template<class Archive>
    void serialize( Archive &ar ) {
      ar( id, is_human, faction );
    }
  };

  inline Texture2D DetermineTextureFromFaction( entt::entity owner ) {
    std::string faction = Global::world.get<Component>( owner ).faction;
    return Global::texture_cache[hstr{
                                   ( faction + "_villager_texture" ).c_str() }]
      ->texture;
  }

  namespace System {

    inline void HumanUpdate( Player::Component & );
    inline void AIUpdate( AI::Component &, Player::Component & );

    inline void Init() {

      std::filesystem::path cwd = std::filesystem::current_path();

      std::cout << "Current Working Directory: " << cwd.generic_string()
                << std::endl;

      std::ifstream f( "app/data/factions.json" );
      {

        nlohmann::json js = nlohmann::json::parse( f );

        std::cout << js << std::endl;
        std::cout << js["romans"] << std::endl;
        std::cout << js["romans"]["adjectival"] << std::endl;
      }
      f.close();

      Global::host_player = Global::world.create();
      Global::world.emplace<Player::Component>(
        Global::host_player, Global::host_player, true, "romans"
      );

      entt::entity ai_player = Global::world.create();
      Global::world.emplace<Player::Component>(
        ai_player, ai_player, false, "celts"
      );
      Global::world.emplace<AI::Component>(
        ai_player, AI::Goal::DevelopSettlements
      );
    }

    inline void Update( View<Player::Component> players ) {
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
          }
          else if ( ai.has_colonist && !ai.has_settlement ) {}
        } break;
      }
    }

  };// namespace System

};// namespace Player
