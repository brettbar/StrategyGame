#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"

#include "../components/actor_component.hpp"
#include "../components/ai_component.hpp"
#include "../components/player_component.hpp"

#include "actor_system.hpp"
#include "commands_system.hpp"
#include "movement_system.hpp"
#include "province_system.hpp"
#include "settlement_system.hpp"
#include <cassert>


namespace AI {

struct System {
  static void Create(entt::entity player) {
    AI::Component ai_c = AI::Component(Goal::None);
    Global::world.emplace<AI::Component>(player, ai_c);
  }

  static void Start() {
    auto ai_players = Global::world.view<Player::Component, AI::Component>();

    for (auto player_e: ai_players) {
      Player::Component player_c = ai_players.get<Player::Component>(player_e);
      AI::Component ai_c = ai_players.get<AI::Component>(player_e);

      determine_goal(player_e, player_c, ai_c);
    }
  }

  static void update(
    entt::entity ai_player,
    Player::Component &player_c,
    AI::Component &ai_c
  ) {
    execute_goals(ai_player, player_c, ai_c);
  }

  static void execute_goals(
    entt::entity ai_player,
    Player::Component &player,
    AI::Component &ai_c
  ) {

    switch (ai_c.current_goal) {
      case Goal::None:
        determine_goal(ai_player, player, ai_c);
        break;
      case Goal::ExpandBorders:
      case Goal::EstablishSettlement: {
        // printf( "player_1 has NOT finished their goal!\n" );

        if (!ai_c.executing_plan) {
          WorldState desired_state = goal_state(ai_c.current_goal);

          sptr<Node> root = std::make_shared<Node>(Node{
            .action =
              Action{
                .type = Action_t::AchieveGoal,
                .preconditions = desired_state,
                .effects = {},
              },
            .children = {},
          });


          WorldState state = init_world_state(ai_player, ai_c);
          build_graph(root, ai_player);


          printf("===== START GRAPH =====\n");
          root->print();
          printf("===== END GRAPH =====\n");

          list<Action> actions = {};

          if (find_a_plan(state, desired_state, actions, root, ai_player)) {
            ai_c.executing_plan = true;
            ai_c.current_plan = Plan{actions, 0};

            printf("===== START PLAN =====\n");
            ai_c.current_plan.print();
            printf("===== END PLAN =====\n");
          }
        } else {
          if (execute_plan(ai_c.current_plan, ai_player)) {
            printf("player_1 has finished their goal!\n");
            ai_c.executing_plan = false;
            ai_c.current_plan = Plan{{}, 0};
            ai_c.current_goal = Goal::None;
          }
        }


      } break;
    }
  }

  static void build_graph(sptr<Node> parent, entt::entity ai_player) {
    for (const auto &[cond_t, value]: parent->action.preconditions) {
      for (auto possible_actions_t: actions_that_satisfy_cond(cond_t)) {
        auto possible_action = get_action(possible_actions_t);

        sptr<Node> new_node = std::make_shared<Node>(Node{
          .action = possible_action,
          .children = {},
        });

        if (parent->children.contains(cond_t)) {
          parent->children.at(cond_t).push_back(new_node);
        } else {
          parent->children.emplace(cond_t, list<sptr<Node>>{new_node});
        }

        // std::cout << "Parent: " << parent->action.as_str() << '\n';
        // std::cout << "Graph node: " << new_node->action.as_str() << '\n';
        // std::cout << std::endl;

        build_graph(new_node, ai_player);
      };
    }
  }

  static bool find_a_plan(
    WorldState &initial_state,
    WorldState desired_state,
    list<Action> &plan,
    sptr<Node> parent,
    entt::entity ai_player
  ) {

    bool all_conds_met = true;

    // I want to find a plan for ExpandBorders
    // This means I need HasSettlements=3

    // We have a graph of potential actions that could be taken
    // First lets just see if the parent's effects are already met
    for (const auto &[cond_t, value]: parent->action.preconditions) {
      ConditionValue curr_val =
        initial_state[cond_t];// Initial::HasSettlements=0
      ConditionValue desired_val =
        desired_state[cond_t];// Desired::HasSettlements=3
      ConditionValue_t value_type = value_type_for_cond_t(cond_t);

      // Check if the conditions are already met
      switch (value_type) {
        case ConditionValue_t::Boolean: {
          if (curr_val.boolean == desired_val.boolean) {
            continue;
          }
        } break;
        case ConditionValue_t::Number: {
          if (curr_val.number == desired_val.number) {
            continue;
          }
        } break;
      }


      if (value_type == ConditionValue_t::Boolean) {
        bool any_valid_action = false;

        for (sptr<Node> child: parent->children.at(cond_t)) {
          auto altered_state = apply_action(initial_state, child->action);

          if (find_a_plan(
                altered_state, desired_state, plan, child, ai_player
              )) {
            any_valid_action = true;
            break;
          }
        }

        if (!any_valid_action) {
          all_conds_met = false;
          break;
        }

      } else if (value_type == ConditionValue_t::Number) {
        u32 desired = desired_val.number;
        u32 current = curr_val.number;
        u32 discrep = desired - current;

        bool any_valid_action = false;

        for (u32 i = 0; i < discrep; i++) {
          for (sptr<Node> child: parent->children.at(cond_t)) {
            auto altered_state = apply_action(initial_state, child->action);

            // Initial::HasSettlements=1
            // Desired::HasSettlements=3

            if (find_a_plan(
                  altered_state, desired_state, plan, child, ai_player
                )) {
              any_valid_action = true;
              break;
            }
          }

          if (!any_valid_action) {
            all_conds_met = false;
            break;
          }
        }
      }
    }

    if (all_conds_met)
      plan.push_back(parent->action);

    return all_conds_met;
  }

  static bool execute_plan(Plan &plan, entt::entity ai_player) {
    //if (plan.stack.size() <= 0) {
    //  return true;
    //}

    //Action action = plan.stack.front();

    //if (action_effects_met(action, ai_player)) {
    //  plan.stack.erase(plan.stack.begin());
    //} else if (action_preconds_met(action, ai_player)) {
    //  do_action(action, ai_player);
    //}
    return false;
  }


  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////
  ///////////////////////

  static WorldState init_world_state(
    entt::entity ai_player,
    AI::Component &ai_c
  ) {
    WorldState world_state = {};
    for (u32 i = 0; i < (u32) Condition_t::COUNT; i++) {
      Condition_t cond = (Condition_t) i;
      world_state[cond] = get_real_state_for_cond(ai_player, cond);
    }

    return world_state;
  }

  static WorldState apply_action(WorldState &state, Action &action) {
    WorldState newState = state;

    for (const auto &[effect, value]: action.effects) {
      switch (value_type_for_cond_t(effect)) {
        case AI::ConditionValue_t::Boolean:
          newState[effect] = value;
          break;
        case AI::ConditionValue_t::Number:
          int current = newState[effect].number;
          newState[effect].number = current + value.number;
          break;
      }
    }

    return newState;
  }


  static ConditionValue get_real_state_for_cond(
    entt::entity ai_player,
    Condition_t cond
  ) {
    switch (cond) {
      case Condition_t::HasColonist: {
        return {
          .boolean =
            Actor::System::get_colonist_of_player(ai_player) != entt::null
        };
      } break;

      case Condition_t::ColonistOnUnclaimedProvince: {
        auto colonist_e = Actor::System::get_colonist_of_player(ai_player);
        if (colonist_e == entt::null)
          return {.boolean = false};

        return {
          .boolean = Actor::System::colonist_can_claim_province(colonist_e)
        };
      } break;

      case Condition_t::ColonistOnOwnProvince: {
        auto colonist_e = Actor::System::get_colonist_of_player(ai_player);
        if (colonist_e == entt::null)
          return {.boolean = false};

        return {
          .boolean = Actor::System::colonist_can_place_settlement(colonist_e)
        };
      } break;

      case Condition_t::HasUnsettledProvince: {
        return {
          .boolean = Province::System::player_has_unsettled_province(ai_player)
        };
      } break;
      case Condition_t::HasSettlements: {
        return {
          .number = Settlement::System::num_player_settlements(ai_player)
        };
      } break;
      case AI::Condition_t::COUNT:
        return {};
    }
  };


  static void determine_goal(
    entt::entity ai_player,
    Player::Component &player,
    AI::Component &ai
  ) {
    assert(
      ai.current_goal == Goal::None
    );// @temp, eventually be able to change goal on the fly

    auto num_player_settlements =
      Settlement::System::num_player_settlements(ai_player);

    if (num_player_settlements < 1) {
      ai.current_goal = Goal::EstablishSettlement;
    } else if (num_player_settlements < 3) {
      ai.current_goal = Goal::ExpandBorders;
    }
  }


  // static bool find_a_plan(
  //   WorldState &state,
  //   sptr<Node> parent,
  //   list<Action> &plan,
  //   entt::entity ai_player
  // ) {
  //   // In order to find a plan, we are looking for a series of actions until
  //   // the final action has all its conditions met

  //   // this function will return true if at least one valid path exists
  //   // otherwise it will return false
  //   bool all_conds_met = true;

  //   for (const auto &[cont_t, value]: parent->action.preconditions) {
  //     auto delta = value.number;

  //     if (condition_met(cond, ai_player)) {
  //       continue;
  //     } else {
  //       bool any_valid_action = false;

  //       for (auto node: parent->children.at(cond)) {
  //         if (find_a_plan(node, plan, ai_player)) {
  //           any_valid_action = true;
  //           break;
  //         }
  //       }

  //       if (!any_valid_action) {
  //         all_conds_met = false;
  //         break;
  //       }
  //     }
  //   }

  //   if (all_conds_met)
  //     plan.push_back(parent->action);

  //   return all_conds_met;
  // }

  // static bool execute_plan(Plan &plan, entt::entity ai_player) {
  //   if (plan.stack.size() <= 0) {
  //     return true;
  //   }

  //   Action action = plan.stack.front();

  //   if (action_effects_met(action, ai_player)) {
  //     plan.stack.erase(plan.stack.begin());
  //   } else if (action_preconds_met(action, ai_player)) {
  //     do_action(action, ai_player);
  //   }
  //   return false;
  // }


  // static bool condition_met(Condition cond, entt::entity ai_player) {
  //   switch (cond.type) {
  //     case Condition_t::HasColonist: {
  //       auto colonist_e = Actor::System::get_colonist_of_player(ai_player);
  //       if (colonist_e == entt::null)
  //         return false;
  //       return true;
  //
  //     } break;
  //
  //     case Condition_t::ColonistOnUnclaimedProvince: {
  //       auto colonist_e = Actor::System::get_colonist_of_player(ai_player);
  //       if (colonist_e == entt::null)
  //         return false;
  //
  //       return Actor::System::colonist_can_claim_province(colonist_e);
  //     } break;
  //
  //     case Condition_t::ColonistOnOwnProvince: {
  //       auto colonist_e = Actor::System::get_colonist_of_player(ai_player);
  //       if (colonist_e == entt::null)
  //         return false;
  //
  //       return Actor::System::colonist_can_place_settlement(colonist_e);
  //     } break;
  //
  //     case Condition_t::HasUnsettledProvince:
  //       return Province::System::player_has_unsettled_province(ai_player);
  //     case Condition_t::HasSettlements: {
  //       return Settlement::System::num_player_settlements(ai_player) >=
  //              cond.data.value;
  //     } break;
  //     case AI::Condition_t::NUM:
  //       break;
  //   }
  //
  //   return false;
  // }
  //
  // static void do_action(Action a, entt::entity ai_player) {
  //
  //   std::cout << "Doing Action :: " << a.as_str() << '\n';
  //   switch (a.type) {
  //     case Action_t::AchieveGoal:
  //       break;
  //     case Action_t::BuildSettlement: {
  //       auto colonist_e = Actor::System::get_colonist_of_player(ai_player);
  //       if (colonist_e == entt::null)
  //         return;
  //
  //       // @todo this is missing a step that is found in campaigns PostCommand
  //       Commands::Manager()->enqueue(
  //         Commands::Command::build_settlement(colonist_e)
  //       );
  //
  //     } break;
  //     case Action_t::ClaimProvince: {
  //       auto colonist_e = Actor::System::get_colonist_of_player(ai_player);
  //       if (colonist_e == entt::null)
  //         return;
  //
  //       Actor::Component actor =
  //         Global::world.get<Actor::Component>(colonist_e);
  //
  //       Commands::Manager()->enqueue(
  //         Commands::Command::claim_province(colonist_e)
  //       );
  //
  //     } break;
  //     case Action_t::MoveColonistToUnsettledOwnedProvince: {
  //       return;
  //     }
  //     case Action_t::MoveColonistToUnclaimedProvince: {
  //       auto colonist_e = Actor::System::get_colonist_of_player(ai_player);
  //       if (colonist_e == entt::null)
  //         return;
  //
  //       Actor::Component actor =
  //         Global::world.get<Actor::Component>(colonist_e);
  //
  //       if (Movement::System::ActorIsMoving(colonist_e))
  //         return;
  //
  //       sptr<vec2f> nearest_eligible_tile =
  //         Province::System::get_nearest_inhabitable_province(actor.position);
  //
  //       if (nearest_eligible_tile) {
  //         Commands::Manager()->enqueue(Commands::Command::move(
  //           ai_player, *nearest_eligible_tile, colonist_e
  //         ));
  //       }
  //
  //
  //     } break;
  //     case Action_t::SpawnColonist: {
  //       vec2f pos =
  //         Settlement::System::position_of_a_player_settlement(ai_player);
  //
  //       Commands::Manager()->enqueue(
  //         Commands::Command::spawn_colonist(ai_player, pos)
  //       );
  //     } break;
  //   }
  // }
  //
  // static u32 get_current_cond_num(
  //   Condition_t cond,
  //   entt::entity ai_player_e
  // ) {
  //   switch (cond) {
  //     case Condition_t::HasSettlements: {
  //       return Settlement::System::num_player_settlements(ai_player_e);
  //     } break;
  //
  //     default:
  //       return 0;
  //   }
  //
  //   return 0;
  // }
  //
  //
  // static bool action_effects_met(Action a, entt::entity ai_player) {
  //   for (auto cond: a.effects) {
  //     if (!condition_met(cond, ai_player)) {
  //       return false;
  //     }
  //   }
  //
  //   return true;
  // }
  //
  // static bool action_preconds_met(Action a, entt::entity ai_player) {
  //   for (auto cond: a.preconditions) {
  //     if (!condition_met(cond, ai_player)) {
  //       return false;
  //     }
  //   }
  //
  //   return true;
  // }
  //
  //
  //
  //
};
};// namespace AI
