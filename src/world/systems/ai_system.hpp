#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"

#include "../components/ai/ai_component.hpp"
#include "../components/player_component.hpp"

#include "actor_system.hpp"
#include "commands_system.hpp"
#include "province_system.hpp"
#include "settlement_system.hpp"
#include <cassert>


namespace AI {

using WorldState = map<Condition_t, ConditionValue>;

struct System {
  static void Create(entt::entity player) {
    AI::Component ai_c = AI::Component(Goal_t::None);
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

  static void determine_goal(
    entt::entity ai_player,
    Player::Component &player,
    AI::Component &ai
  ) {
    assert(
      ai.current_goal == Goal_t::None
    );// @temp, eventually be able to change goal on the fly

    auto num_player_settlements =
      Settlement::System::num_player_settlements(ai_player);

    if (num_player_settlements < 1) {
      ai.current_goal = Goal_t::EstablishSettlement;
      printf("Starting goal Goal::EstablishSettlement\n");
    } else if (num_player_settlements < 3) {
      ai.current_goal = Goal_t::ExpandBorders;
      printf("Starting goal Goal::ExpandBorders\n");
    }
  }


  static WorldState current_world_state(
    entt::entity ai_player,
    AI::Component &ai_c
  ) {
    WorldState world_state = {};
    for (u32 i = 0; i < (u32) Condition_t::COUNT; i++) {
      Condition_t cond = (Condition_t) i;
      world_state[cond] = ConditionValue{
        get_real_state_for_cond(ai_player, cond),
      };
    }

    return world_state;
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
    Player::Component &player_c,
    AI::Component &ai_c
  ) {

    switch (ai_c.current_goal) {
      case Goal_t::None:
        determine_goal(ai_player, player_c, ai_c);
        break;
      case Goal_t::ExpandBorders:
      case Goal_t::EstablishSettlement: {
        // printf( "player_1 has NOT finished their goal!\n" );

        WorldState state = current_world_state(ai_player, ai_c);
        Goal current_goal = goal(ai_c.current_goal);
        list<Condition> goal_conds = current_goal.conditions;

        if (!ai_c.executing_plan) {
          sptr<Node> root = std::make_shared<Node>(Node{
            .action =
              Action{
                .type = Action_t::AchieveGoal,
                .preconditions = goal_conds,
                .effects = {},
              },
            .children = {},
          });

          build_graph(root, ai_player);

          printf("===== START GRAPH =====\n");
          root->print();
          printf("===== END GRAPH =====\n");

          list<Action> actions = {};

          if (find_a_plan(state, goal_conds, actions, root, ai_player)) {
            ai_c.executing_plan = true;
            ai_c.current_plan = Plan{actions, 0};

            printf("===== START PLAN =====\n");
            ai_c.current_plan.print();
            printf("===== END PLAN =====\n");
          }
        } else {
          if (execute_plan(
                state, goal_conds, ai_c.current_plan, ai_player, ai_c
              )) {
            printf("player_1 has finished their goal!\n");
            ai_c.executing_plan = false;
            ai_c.current_goal = Goal_t::None;
            ai_c.current_plan = Plan{{}, 0};
            ai_c.current_action = nullptr;
          }
        }


      } break;
    }
  }

  static void build_graph(sptr<Node> parent, entt::entity ai_player) {
    for (Condition condition: parent->action.preconditions) {
      for (auto possible_actions_t: actions_that_satisfy_cond(condition.type)) {
        auto possible_action = get_action(possible_actions_t);

        sptr<Node> new_node = std::make_shared<Node>(Node{
          .action = possible_action,
          .children = {},
        });

        if (parent->children.contains(condition.type)) {
          parent->children.at(condition.type).push_back(new_node);
        } else {
          parent->children.emplace(condition.type, list<sptr<Node>>{new_node});
        }

        build_graph(new_node, ai_player);
      };
    }
  }

  static bool find_a_plan(
    WorldState &current_state,
    list<Condition> goal_conds,
    list<Action> &plan,
    sptr<Node> parent,
    entt::entity ai_player
  ) {
    bool all_action_conds_met = true;
    for (Condition condition: parent->action.preconditions) {
      printf("Analyzing Precondition %s\n", condition.as_str().c_str());

      // If the precondition is met, no need to recurse further
      if (condition_met(current_state, condition)) {
        continue;
      }

      list<sptr<Node>> children_that_satisfy_cond =
        parent->children[condition.type];

      bool any_valid_action = false;

      // Only one of these needs to work
      for (sptr<Node> child: children_that_satisfy_cond) {
        // TODO probably need to modify the current_state here for accumatlive actions
        apply_action(current_state, child->action);

        if (find_a_plan(current_state, goal_conds, plan, child, ai_player)) {
          any_valid_action = true;
          break;
        }
      }

      // If for this precondition we have found no valid actions
      // The whole of this action is not possible
      if (!any_valid_action) {
        all_action_conds_met = false;
        break;
      }
    }

    if (all_action_conds_met)
      plan.push_back(parent->action);

    return all_action_conds_met;
  }


  static bool execute_plan(
    WorldState state,
    list<Condition> goal_conds,
    Plan &plan,
    entt::entity ai_player,
    AI::Component &ai_c
  ) {
    if (plan.stack.size() <= 0) {
      return true;
    }

    bool new_action = false;

    if (ai_c.current_action == nullptr) {
      ai_c.current_action = std::make_shared<Action>(plan.stack.front());

      new_action = true;
    }

    if (all_action_effects_met(state, *ai_c.current_action)) {
      plan.stack.erase(plan.stack.begin());
      ai_c.current_action = nullptr;
    } else if (action_preconds_met(state, *ai_c.current_action)) {
      if (new_action) {
        printf("Executing Action %s\n", ai_c.current_action->as_str().c_str());
        do_action(*ai_c.current_action, ai_player);
      }
    }

    return false;
  }

  static bool condition_met(WorldState current_state, Condition &cond) {
    ConditionValue current = current_state[cond.type];
    bool met = cond.is_satisfied_by(current);
    return met;
  }

  static bool all_action_effects_met(WorldState state, Action action) {
    for (Effect effect: action.effects) {
      ConditionValue expected_before = effect.before;
      ConditionValue expected_after = effect.after;

      ConditionValue current = state[effect.type];

      switch (value_type_for_cond_t(effect.type)) {
        case AI::ConditionValue_t::Boolean:
          return expected_after.boolean == current.boolean;
        case AI::ConditionValue_t::Number:
          // @TODO this is almost certainly wrong(or at least just incomplete)
          // Like what if we wanted to go from 4 to 3, then this should be decrementing
          // So we need to take into account what the effect op was.
          // if it was increasing, then maybe this suffices
          // if it was supposed to be decreasing, then maybe <= or something?
          // if set, maybe ==
          return expected_after.number >= current.number;
      }
    }

    return true;
  }

  static bool action_preconds_met(WorldState &state, Action a) {
    for (Condition cond: a.preconditions) {
      if (!condition_met(state, cond)) {
        return false;
      }
    }

    return true;
  }


  static void apply_action(WorldState &state, Action &action) {

    for (Effect &effect: action.effects) {
      effect.before = state[effect.type];

      switch (value_type_for_cond_t(effect.type)) {
          // @todo  actually use the op
        case AI::ConditionValue_t::Boolean:
          state[effect.type].boolean = effect.value.boolean;
          break;
        case AI::ConditionValue_t::Number:
          int current = state[effect.type].number;
          state[effect.type].number = current + effect.value.number;
          break;
      }

      effect.after = state[effect.type];
    }
  }


  static void do_action(Action a, entt::entity ai_player) {

    std::cout << "Doing Action :: " << a.as_str() << '\n';
    switch (a.type) {
      case Action_t::AchieveGoal:
        break;
      case Action_t::BuildSettlement: {
        auto colonist_e = Actor::System::get_colonist_of_player(ai_player);
        if (colonist_e == entt::null)
          return;

        // @todo this is missing a step that is found in campaigns PostCommand
        Commands::Manager()->enqueue(
          Commands::Command::build_settlement(colonist_e)
        );

      } break;
      case Action_t::ClaimProvince: {
        auto colonist_e = Actor::System::get_colonist_of_player(ai_player);
        if (colonist_e == entt::null)
          return;

        Actor::Component actor =
          Global::world.get<Actor::Component>(colonist_e);

        Commands::Manager()->enqueue(
          Commands::Command::claim_province(colonist_e)
        );

      } break;
      case Action_t::MoveColonistToUnsettledOwnedProvince: {
        return;
      }
      case Action_t::MoveColonistToUnclaimedProvince: {
        auto colonist_e = Actor::System::get_colonist_of_player(ai_player);
        if (colonist_e == entt::null)
          return;

        Actor::Component actor =
          Global::world.get<Actor::Component>(colonist_e);

        if (Movement::System::ActorIsMoving(colonist_e))
          return;

        sptr<vec2f> nearest_eligible_tile =
          Province::System::get_nearest_inhabitable_province(actor.position);

        if (nearest_eligible_tile) {
          Commands::Manager()->enqueue(
            Commands::Command::move(
              ai_player, *nearest_eligible_tile, colonist_e
            )
          );
        }


      } break;
      case Action_t::SpawnColonist: {
        vec2f pos =
          Settlement::System::position_of_a_player_settlement(ai_player);

        Commands::Manager()->enqueue(
          Commands::Command::spawn_colonist(ai_player, pos)
        );
      } break;
    }
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
  //
  //
  //
  //
};

};// namespace AI
