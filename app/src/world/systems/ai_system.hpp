#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../components/ai.hpp"
#include "../components/player.hpp"
#include "../components/province.hpp"
#include "../components/settlement.hpp"

namespace AI {
  inline Goal goal( entt::entity ai_player, Goal_t goal_t ) {
    switch ( goal_t ) {
      case Goal_t::None:
        return Goal{};
      case Goal_t::EstablishSettlement:
        return Goal{
          .goal = Goal_t::EstablishSettlement,
          .conditions =
            {
              Condition{
                .condition = Condition_t::HasSettlement,
                .is_satisified = [ai_player]() -> bool {
                  auto settlements =
                    Global::world
                      .view<Province::Component, Settlement::Component>();

                  for ( auto settlement_e: settlements ) {
                    auto prov =
                      settlements.get<Province::Component>( settlement_e );
                    if ( prov.tile->owner == ai_player ) {
                      return true;
                    }
                  }

                  return false;
                },
              },
            },
        };
    }
  }

  inline void Create( entt::entity player ) {
    Global::world.emplace<AI::Component>(
      player, Goal{ .goal = Goal_t::None }
    );
  }

  inline void Start() {
    auto ai_players = Global::world.view<Player::Component, AI::Component>();

    for ( auto player_e: ai_players ) {
      auto ai = ai_players.get<AI::Component>( player_e );

      ai.current_goal = goal( player_e, Goal_t::EstablishSettlement );
    }
  }

  inline void find_best_plan( Goal goal ) {
    Action action;
  }

  inline void CheckGoals( entt::entity ai_player ) {
    auto player = Global::world.get<Player::Component>( ai_player );
    auto &ai = Global::world.get<AI::Component>( ai_player );

    switch ( ai.current_goal.goal ) {
      case Goal_t::None:
        ai.current_goal = Goal{ Goal_t::EstablishSettlement };
        break;
      case Goal_t::EstablishSettlement:
        if ( ai.current_goal.succeeded() ) {
          printf( "player_1 has finished their goal!\n" );
        } else {
          printf( "player_1 has NOT finished their goal!\n" );
        }
        break;
    }
  }


  // inline void Planner() {
  //   auto ai_players = Global::world.view<Player::Component, AI::Component>();

  //   for ( auto player_e: ai_players ) {
  //     auto ai = ai_players.get<AI::Component>( player_e );

  //     switch ( ai.current_goal ) {
  //       case Goal_t::None:
  //         ai.current_goal = Goal_t::EstablishSettlement;
  //         break;
  //       case Goal_t::EstablishSettlement: {

  //       } break;
  //     }
  //   }
  // }
};// namespace AI
