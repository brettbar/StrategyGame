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
    Global::world.emplace<AI::Component>( player, Goal::None );
  }

  inline void Start() {
    auto ai_players = Global::world.view<Player::Component, AI::Component>();

    for ( auto player_e: ai_players ) {
      auto ai = ai_players.get<AI::Component>( player_e );

      ai.current_goal = Goal::EstablishSettlement;
    }
  }

  inline bool condition_met( Condition cond, entt::entity ai_player ) {
    switch ( cond ) {
      case Condition::HasColonist: {
        auto colonist_e = Actor::System::get_colonist_of_player( ai_player );
        if ( colonist_e == entt::null )
          return false;
        return true;

      } break;

      case Condition::ColonistOnUnclaimedProvince: {
        auto colonist_e = Actor::System::get_colonist_of_player( ai_player );
        if ( colonist_e == entt::null )
          return false;
        return true;

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

  inline void do_action( Action a, entt::entity ai_player ) {
    switch ( a.type ) {
      case Action_t::BuildSettlement: {
        auto colonist_e = Actor::System::get_colonist_of_player( ai_player );
        if ( colonist_e == entt::null )
          return;

        // @todo this is missing a step that is found in campaigns PostCommand
        Commands::Manager()->enqueue(
          Commands::Command::build_settlement( colonist_e )
        );

      } break;
      case Action_t::ClaimProvince: {
        auto colonist_e = Actor::System::get_colonist_of_player( ai_player );
        if ( colonist_e == entt::null )
          return;

        Actor::Component actor =
          Global::world.get<Actor::Component>( colonist_e );

        Commands::Manager()->enqueue(
          Commands::Command::claim_province( colonist_e )
        );

      } break;
      case Action_t::MoveColonistToUnclaimedProvince: {
        auto colonist_e = Actor::System::get_colonist_of_player( ai_player );
        if ( colonist_e == entt::null )
          return;

        Actor::Component actor =
          Global::world.get<Actor::Component>( colonist_e );

        Commands::Manager()->enqueue( Commands::Command::move(
          ai_player,
          { actor.position.x + 128, actor.position.y + 128 },
          colonist_e
        ) );

      } break;
      case Action_t::SpawnColonist:
        break;
    }
  }

  inline bool action_effects_met( Action a, entt::entity ai_player ) {
    for ( auto cond: a.effects ) {
      if ( !condition_met( cond, ai_player ) ) {
        return false;
      }
    }

    return true;
  }

  inline bool action_preconds_met( Action a, entt::entity ai_player ) {
    for ( auto cond: a.preconditions ) {
      if ( !condition_met( cond, ai_player ) ) {
        return false;
      }
    }

    return true;
  }

  inline list<Action_t> actions_that_satisfy_cond( Condition cond ) {
    switch ( cond ) {
      case Condition::ColonistOnUnclaimedProvince:
        return { Action_t::MoveColonistToUnclaimedProvince };
      case Condition::ColonistOnOwnProvince:
        return { Action_t::MoveColonistToOwnProvince };
      case Condition::HasColonist:
        return { Action_t::SpawnColonist };
      case Condition::HasProvince:
        return { Action_t::ClaimProvince };
      case Condition::HasSettlement:
        return { Action_t::BuildSettlement };
    }
  };


  inline void build_graph( Node &parent, entt::entity ai_player ) {
    for ( auto cond: parent.action.preconditions ) {
      for ( auto possible_actions_t: actions_that_satisfy_cond( cond ) ) {
        auto possible_action = get_action( possible_actions_t );

        Node new_node = {
          .action = possible_action,
          .cost = 0,
          .children = {},
        };

        parent.children.push_back( new_node );

        build_graph( new_node, ai_player );
      };
    }
  }

  inline bool find_best_plan( Node &parent, entt::entity ai_player ) {
    for ( auto cond: parent.action.preconditions ) {
      if ( !condition_met( cond, ai_player ) ) {}
    }
  }

  inline void make_plan( Goal goal, entt::entity ai_player ) {
    Node root = {
      .action =
        {
          .type = Action_t::AchieveGoal,
          .preconditions = goal_conds( goal ),
          .effects = {},
        },
      .cost = 0,
      .children = {},
    };

    build_graph( root, ai_player );
  }

  // inline void execute_plan( Plan plan, entt::entity ai_player ) {
  //   auto &ai = Global::world.get<AI::Component>( ai_player );

  //   if ( plan.stack.size() > 0 ) {
  //     Action a = plan.peek();

  //     if ( action_effects_met( a, ai_player ) ) {
  //       plan.pop();
  //     } else {
  //       if ( action_preconds_met( a, ai_player ) ) {
  //         do_action( a, ai_player );
  //       } else {
  //         printf( "Plan no longer possible!\n" );
  //         ai.current_plan = {};
  //         ai.executing_plan = false;
  //         return;
  //       }
  //     }
  //   }

  //   if ( plan.stack.size() <= 0 ) {
  //     printf( "Plan finished!\n" );
  //     ai.current_plan = {};
  //     ai.executing_plan = false;
  //     return;
  //   }
  // }

  // inline void planner( Goal goal, entt::entity ai_player ) {
  //   /*
  //     Starting at the root (goal) we want to start building a tree of
  //     actions that satisfy the condition of that goal

  //     Let say we have a goal EstablishSettlement. This goal has as the HasSettlement condition.
  //     So the first step is to acheive this goal is to check this condition. If the condition is true,
  //     we are done and have met our goal. Otherwise, we need to find the actions who's effects can satisfy
  //     this condition.

  //     Lets say there exist 2 actions who's effects satisfy HasSettlement; BuildSettlement and CaptureSettlement.
  //     So the first step would be to check the preconditions of BuildSettlement. If all of those conditions are true
  //     then we know that action is ready to fire, and we have a valid action path (plan) that the ai can execute.
  //   */


  //   // We need to make a list of plans
  //   // A plan is a list of actions the first of which is eligible that lead to completing the goal
  //   // If we get more than one possible plans, take the cheapest one
  //   // If we get no possible plans, we cant proceed with this goal currently


  //   list<Plan> plans = {};

  //   bool at_least_one_plan = make_plans( goal.desired_state, ai_player, plans );

  //   if ( at_least_one_plan ) {
  //     Plan cheapest_plan = get_cheapest_plan( plans );
  //     auto &ai = Global::world.get<AI::Component>( ai_player );
  //     ai.executing_plan = true;
  //     ai.current_plan = cheapest_plan;
  //   }
  // }


  // inline void execute_goals( entt::entity ai_player ) {
  //   auto player = Global::world.get<Player::Component>( ai_player );
  //   auto &ai = Global::world.get<AI::Component>( ai_player );

  //   switch ( ai.current_goal.goal ) {
  //     case Goal::None:
  //       if ( !SettlementSystem::player_has_settlement( ai_player ) )
  //         ai.current_goal = goal( Goal::EstablishSettlement );

  //       break;
  //     case Goal::EstablishSettlement: {
  //       // printf( "player_1 has NOT finished their goal!\n" );


  //       if ( !ai.executing_plan ) {
  //         // ai.current_plan = make_plan( ai.current_goal, ai_player );
  //         planner( ai.current_goal, ai_player );
  //       } else {
  //         execute_plan( ai.current_plan, ai_player );
  //       }


  //     } break;
  //   }
  // }
};// namespace AI
