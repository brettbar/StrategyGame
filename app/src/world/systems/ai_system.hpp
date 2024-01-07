#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../components/actor.hpp"
#include "../components/ai.hpp"
#include "../components/player.hpp"
#include "../components/province.hpp"
#include "../components/settlement.hpp"
#include "actor_system.hpp"
#include "commands.hpp"
#include "province_system.hpp"
#include "settlement_system.hpp"
#include <condition_variable>


namespace AI {

  inline void Create( entt::entity player ) {
    Global::world.emplace<AI::Component>(
      player, Goal{ .goal = Goal_t::None }
    );
  }

  inline bool condition_met( Condition cond, entt::entity ai_player ) {
    switch ( cond ) {
      case Condition::HasColonistOnEligibleTerrain: {
        auto colonist_e = Actor::System::get_colonist_of_player( ai_player );

        if ( colonist_e == entt::null )
          return false;

        return Actor::System::colonist_can_claim_province( colonist_e );

      } break;


      case Condition::HasProvince:
        return ProvinceSystem::player_has_province( ai_player );
      case Condition::HasSettlement:
        return SettlementSystem::player_has_settlement( ai_player );
      case Condition::None:
        return true;
    }

    return false;
  }

  inline bool do_action( Action a, entt::entity ai_player ) {
    switch ( a.type ) {
      case Action_t::BuildSettlement: {
        auto colonist_e = Actor::System::get_colonist_of_player( ai_player );
        if ( colonist_e == entt::null )
          return false;

        Actor::Component actor =
          Global::world.get<Actor::Component>( colonist_e );

        // @todo this is missing a step that is found in campaigns PostCommand
        Commands::Manager()->enqueue( Commands::Command{
          .type = Commands::Type::BuildSettlement,
          .msg = "Player spawn Settlement",
          .entity = colonist_e,
        } );


        // @todo did it actually succeed
        return true;
      } break;
      case Action_t::ClaimProvince: {
        auto colonist_e = Actor::System::get_colonist_of_player( ai_player );

        if ( colonist_e == entt::null )
          return false;

        Actor::Component actor =
          Global::world.get<Actor::Component>( colonist_e );

        Commands::Manager()->enqueue( {
          Commands::Type::ClaimProvince,
          ai_player,
          "Player taking ownership of Province",
          actor.position,
        } );

        // @todo did it actually succeed
        return true;
      } break;
      case Action_t::SpawnColonist:
        return true;
    }

    return false;
  }

  inline Action action( Action_t type ) {
    switch ( type ) {
      case Action_t::BuildSettlement:
        return Action{
          .type = type,
          .cost = 0,
          .preconditions =
            {
              Condition::HasProvince,
            },
          .effect = Condition::HasSettlement,
        };
      case Action_t::ClaimProvince:
        return Action{
          .type = type,
          .cost = 0,
          .preconditions = { Condition::HasColonistOnEligibleTerrain },
          .effect = Condition::HasProvince,
        };
      case Action_t::SpawnColonist:
        return Action{
          .type = type,
          .cost = 0,
          // @todo requirements to make colonist
          .preconditions{},
          .effect = Condition::HasColonistOnEligibleTerrain,
        };

      default:
        // bad
        return Action{
          .type = Action_t::DoNothing,
          .cost = 0,
          .preconditions = {},
          .effect = Condition::None,
        };
    }
  };

  inline Goal goal( Goal_t goal_t ) {
    switch ( goal_t ) {
      case Goal_t::None:
        return Goal{
          .goal = Goal_t::None,
          .desired_state = {},
        };
      case Goal_t::EstablishSettlement:
        return Goal{
          .goal = Goal_t::EstablishSettlement,
          .desired_state =
            {
              Condition::HasSettlement,
            },
        };
    }
  }

  inline bool all_conds_met_for_action( Action a, entt::entity ai_player ) {
    for ( auto cond: a.preconditions ) {
      if ( !condition_met( cond, ai_player ) ) {
        return false;
      }
    }

    return true;
  }

  inline list<Action_t> actions_that_satisfy_cond( Condition cond ) {
    switch ( cond ) {
      case Condition::None:
        return {};
      case Condition::HasColonistOnEligibleTerrain:
        return { Action_t::SpawnColonist };
      case Condition::HasProvince:
        return { Action_t::ClaimProvince };
      case Condition::HasSettlement:
        return { Action_t::BuildSettlement };
    }
  };

  inline void find_action_for_cond( Condition cond, list<Action> &plan ) {
    auto possible_actions = actions_that_satisfy_cond( cond );

    for ( auto action_t: possible_actions ) {
      auto action_node = action( action_t );

      plan.push_back( action_node );

      for ( auto precond: action_node.preconditions ) {
        find_action_for_cond( precond, plan );
      }
    }
  }

  inline Plan make_plan( Goal goal, entt::entity ai_player ) {
    // 1. Create action tree starting with the goal's cond as the first node
    // 2. Evaluate cost of each branch
    // 3. Pick the cheapest branch that is possible
    // 4. If none are possible, we can't complete the goal currently

    list<Action> plan = {};

    for ( auto cond: goal.desired_state ) {
      find_action_for_cond( cond, plan );
    }

    return Plan{ plan };
  }

  inline bool execute_plan( Plan plan, entt::entity ai_player ) {
    while ( plan.stack.size() > 0 ) {
      Action a = plan.pop();
      // printf( "Action: %d\n", a.type );

      if ( all_conds_met_for_action( a, ai_player ) ) {
        if ( !do_action( a, ai_player ) )
          return false;
      }
    }

    return true;
  }


  inline void Start() {
    auto ai_players = Global::world.view<Player::Component, AI::Component>();

    for ( auto player_e: ai_players ) {
      auto ai = ai_players.get<AI::Component>( player_e );

      ai.current_goal = goal( Goal_t::EstablishSettlement );
    }
  }


  inline void CheckGoals( entt::entity ai_player ) {
    auto player = Global::world.get<Player::Component>( ai_player );
    auto &ai = Global::world.get<AI::Component>( ai_player );

    switch ( ai.current_goal.goal ) {
      case Goal_t::None:
        if ( !SettlementSystem::player_has_settlement( ai_player ) )
          ai.current_goal = goal( Goal_t::EstablishSettlement );

        break;
      case Goal_t::EstablishSettlement: {
        // printf( "player_1 has NOT finished their goal!\n" );

        Plan plan = make_plan( ai.current_goal, ai_player );

        if ( execute_plan( plan, ai_player ) ) {
          printf(
            "player_1 has finished their goal of %d!\n", ai.current_goal.goal
          );

          ai.current_goal = goal( Goal_t::None );
        }

      } break;
    }
  }

};// namespace AI
