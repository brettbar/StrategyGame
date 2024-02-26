#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../components/actor.hpp"
#include "../components/ai.hpp"
#include "../components/player.hpp"
#include "../components/province.hpp"
#include "../components/settlement.hpp"
#include "../systems/actor.hpp"
#include "commands.hpp"
#include "movement_system.hpp"
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

        return Actor::System::colonist_can_claim_province( colonist_e );
      } break;

      case Condition::ColonistOnOwnProvince: {
        auto colonist_e = Actor::System::get_colonist_of_player( ai_player );
        if ( colonist_e == entt::null )
          return false;

        return Actor::System::colonist_can_place_settlement( colonist_e );
      } break;

      case Condition::HasProvince:
        return ProvinceSystem::player_has_province( ai_player );
      case Condition::HasSettlement:
        return SettlementSystem::player_has_settlement( ai_player );
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
      case Action_t::MoveColonistToOwnProvince: {
        return;
      }
      case Action_t::MoveColonistToUnclaimedProvince: {
        auto colonist_e = Actor::System::get_colonist_of_player( ai_player );
        if ( colonist_e == entt::null )
          return;

        Actor::Component actor =
          Global::world.get<Actor::Component>( colonist_e );

        if ( MovementSystem::ActorIsMoving( colonist_e ) )
          return;

        auto nearest_eligible_tile =
          ProvinceSystem::get_nearest_inhabitable_province( actor.position );

        if ( nearest_eligible_tile )
          Commands::Manager()->enqueue( Commands::Command::move(
            ai_player, *nearest_eligible_tile, colonist_e
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


  inline void build_graph( sptr<Node> parent, entt::entity ai_player ) {

    for ( auto cond: parent->action.preconditions ) {

      for ( auto possible_actions_t: actions_that_satisfy_cond( cond ) ) {
        auto possible_action = get_action( possible_actions_t );

        sptr<Node> new_node = std::make_shared<Node>( Node{
          .action = possible_action,
          .cost = 0,
          .children = {},
        } );

        if ( parent->children.contains( cond ) ) {
          parent->children.at( cond ).push_back( new_node );
        } else {
          parent->children.emplace( cond, list<sptr<Node>>{ new_node } );
        }

        // std::cout << "Parent: " << parent->action.as_str() << '\n';
        // std::cout << "Graph node: " << new_node->action.as_str() << '\n';
        // std::cout << std::endl;

        build_graph( new_node, ai_player );
      };
    }
  }

  inline bool find_a_plan(
    sptr<Node> parent,
    list<Action> &plan,
    entt::entity ai_player
  ) {
    // In order to find a plan, we are looking for a series of actions until
    // the final action has all its conditions met

    // this function will return true if at least one valid path exists
    // otherwise it will return false
    bool all_conds_met = true;

    for ( auto cond: parent->action.preconditions ) {
      if ( condition_met( cond, ai_player ) ) {
        continue;
      } else {
        bool any_valid_action = false;

        for ( auto node: parent->children.at( cond ) ) {
          if ( find_a_plan( node, plan, ai_player ) ) {
            any_valid_action = true;
            break;
          }
        }

        if ( !any_valid_action ) {
          all_conds_met = false;
          break;
        }
      }
    }

    if ( all_conds_met )
      plan.push_back( parent->action );

    return all_conds_met;
  }

  inline bool execute_plan( Plan &plan, entt::entity ai_player ) {
    if ( plan.stack.size() <= 0 ) {
      return true;
    }

    Action action = plan.stack.front();

    if ( action_effects_met( action, ai_player ) ) {
      std::cout << "Action " << action.as_str() << '\n';
      plan.stack.erase( plan.stack.begin() );
    } else if ( action_preconds_met( action, ai_player ) ) {
      do_action( action, ai_player );
    }
    return false;
  }

  inline void execute_goals( entt::entity ai_player ) {
    auto player = Global::world.get<Player::Component>( ai_player );
    auto &ai = Global::world.get<AI::Component>( ai_player );

    switch ( ai.current_goal ) {
      case Goal::None:
        if ( !SettlementSystem::player_has_settlement( ai_player ) )
          ai.current_goal = Goal::EstablishSettlement;

        break;
      case Goal::EstablishSettlement: {
        // printf( "player_1 has NOT finished their goal!\n" );


        if ( !ai.executing_plan ) {
          sptr<Node> root = std::make_shared<Node>( Node{
            .action =
              Action{
                .type = Action_t::AchieveGoal,
                .preconditions = goal_conds( ai.current_goal ),
                .effects = {},
              },
            .cost = 0,
            .children = {},
          } );

          build_graph( root, ai_player );

          root->print();


          list<Action> actions = {};

          bool found_plan = find_a_plan( root, actions, ai_player );

          if ( found_plan ) {
            ai.executing_plan = true;
            ai.current_plan = Plan{ actions, 0 };
          }
        } else {
          if ( execute_plan( ai.current_plan, ai_player ) ) {
            printf( "player_1 has finished their goal!\n" );
            ai.executing_plan = false;
            ai.current_plan = Plan{ {}, 0 };
            ai.current_goal = Goal::None;
          }
        }


      } break;
    }
  }
};// namespace AI
