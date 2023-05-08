#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../components/ai.hpp"
#include "../components/player.hpp"


namespace PlayerSystem
{
  inline void HumanUpdate( Player::Component & );
  inline void AIUpdate( AI::Component &, Player::Component & );

  inline void Update( View<Player::Component> players )
  {
    // Change to exclude AI
    for ( auto entity: players )
    {
      Player::Component &player =
        Global::world.get<Player::Component>( entity );

      if ( player.is_human )
      {
        HumanUpdate( player );
      }
    }

    for ( auto entity: Global::world.view<Player::Component, AI::Component>() )
    {
      Player::Component &player =
        Global::world.get<Player::Component>( entity );
      AI::Component &ai = Global::world.get<AI::Component>( entity );

      AIUpdate( ai, player );
    }
  }

  inline void HumanUpdate( Player::Component &player ) {}

  inline void AIUpdate( AI::Component &ai, Player::Component &player )
  {
    switch ( ai.current_goal )
    {
      case AI::Goal::DevelopSettlements:
      {
        if ( !ai.has_colonist )
        {
          // ActorSystem::SpawnColonist( player.id );
          ai.has_colonist = true;
        }
        else if ( ai.has_colonist && !ai.has_settlement )
        {
        }
      }
      break;
    }
  }


};// namespace PlayerSystem
