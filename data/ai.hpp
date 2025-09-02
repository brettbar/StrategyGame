#pragma once

#include "vector"
#include <string>

namespace AI {
  /*
    The ultimate final goal a player will have is to become the most
    'powerful' in the game. They can do this in the following ways:

  - Martial Victory:
    Having the strongest army for X number of years,
    greater than the all other factions combined

  - Economic Victory:
    Make all other factions completely reliant on your trade and industry

  - Diplomatic Victory:
    Achieve peace on the continent by allying those who will, and destroying
    those who wont

  - Civic Victory: 
    Maintain the largest population for X number of years
    Can increase by player wide or tall
  */


  enum class Goal {
    // BoostProduction,
    // DevelopSettlements,
    None,
    EstablishSettlement,
    ExpandBorders,
  };
  enum class Action_t {
    AchieveGoal,
    MoveColonistToUnclaimedProvince,
    MoveColonistToOwnProvince,
    SpawnColonist,
    ClaimProvince,
    BuildSettlement,
  };
  enum class Condition {
    ColonistOnUnclaimedProvince,
    ColonistOnOwnProvince,
    HasColonist,
    HasProvince,
    HasSettlement,
  };

  struct Action {
    Action_t type;

    float cost = 0;

    std::vector<Condition> preconditions;
    std::vector<Condition> effects;

    std::string as_str() {
      switch (type) {
        case Action_t::AchieveGoal:
          return "AchieveGoal";
        case Action_t::MoveColonistToUnclaimedProvince:
          return "MoveColonistToUnclaimedProvince";
        case Action_t::MoveColonistToOwnProvince:
          return "MoveColonistToOwnProvince";
        case Action_t::SpawnColonist:
          return "SpawnColonist";
        case Action_t::ClaimProvince:
          return "ClaimProvince";
        case Action_t::BuildSettlement:
          return "BuildSettlement";
      }
    }
  };

  inline Action get_action(Action_t type) {
    switch (type) {
      case Action_t::AchieveGoal:
        return Action{
          .type = type,
          .preconditions = {},
          .effects = {},
        };
      case Action_t::BuildSettlement:
        return Action{
          .type = type,
          .preconditions =
            {
              Condition::ColonistOnOwnProvince,
            },
          .effects = {Condition::HasSettlement},
        };
      case Action_t::ClaimProvince:
        return Action{
          .type = type,
          .preconditions = {Condition::ColonistOnUnclaimedProvince},
          .effects = {Condition::HasProvince},
        };
      case Action_t::SpawnColonist:
        return Action{
          .type = type,
          // @todo requirements to make colonist
          .preconditions{},
          .effects = {Condition::HasColonist},
        };

      case Action_t::MoveColonistToUnclaimedProvince:
        return Action{
          .type = type,
          .preconditions = {Condition::HasColonist},
          .effects = {Condition::ColonistOnUnclaimedProvince},
        };
      case Action_t::MoveColonistToOwnProvince:
        return Action{
          .type = type,
          .preconditions =
            {
              Condition::HasColonist,
              Condition::HasProvince,
            },
          .effects = {Condition::ColonistOnOwnProvince},
        };
    }
  };

  inline std::vector<Condition> goal_conds(Goal goal) {
    switch (goal) {
      case Goal::None:
        return {};
      case Goal::EstablishSettlement:
        return {Condition::HasSettlement};
      case Goal::ExpandBorders:
        return {};// @todo
    }
  }

  inline float default_goal_prio(Goal goal) {
    switch (goal) {
      case Goal::None:
        return 0;
      case Goal::EstablishSettlement:
        return 1;
      case Goal::ExpandBorders:
        return 0.5;
    }
  }


  inline std::vector<Action_t> actions_that_satisfy_cond(Condition cond) {
    switch (cond) {
      case Condition::ColonistOnUnclaimedProvince:
        return {Action_t::MoveColonistToUnclaimedProvince};
      case Condition::ColonistOnOwnProvince:
        return {Action_t::MoveColonistToOwnProvince};
      case Condition::HasColonist:
        return {Action_t::SpawnColonist};
      case Condition::HasProvince:
        return {Action_t::ClaimProvince};
      case Condition::HasSettlement:
        return {Action_t::BuildSettlement};
    }
  }

}// namespace AI