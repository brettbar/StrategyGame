#pragma once

#include "../../../shared/common.hpp"
#include "condition.hpp"

namespace AI {
enum class Action_t {
  AchieveGoal,
  MoveColonistToUnclaimedProvince,
  MoveColonistToUnsettledOwnedProvince,
  SpawnColonist,
  ClaimProvince,
  BuildSettlement,
};

struct Action {
  Action_t type;

  float cost = 0;

  list<Condition> preconditions;
  list<State> effects;

  str as_str() {
    switch (type) {
      case Action_t::AchieveGoal:
        return "AchieveGoal";
      case Action_t::MoveColonistToUnclaimedProvince:
        return "MoveColonistToUnclaimedProvince";
      case Action_t::MoveColonistToUnsettledOwnedProvince:
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
            {
              Condition_t::ColonistOnOwnProvince,
              ConditionCompare::Equals,
              {.boolean = true},
            },
          },
        .effects = {
          {Condition_t::HasSettlements, {.number = 1}},
        },
      };
    case Action_t::ClaimProvince:
      return Action{
        .type = type,
        .preconditions =
          {
            {
              Condition_t::ColonistOnUnclaimedProvince,
              ConditionCompare::Equals,
              {.boolean = true},
            },
          },
        .effects = {
          {Condition_t::HasUnsettledProvince, {.boolean = true}},
        },
      };
    case Action_t::SpawnColonist:
      return Action{
        .type = type,
        // @todo requirements to make colonist
        .preconditions{},
        .effects = {
          {Condition_t::HasColonist, {.boolean = true}},
        },
      };

    case Action_t::MoveColonistToUnclaimedProvince:
      return Action{
        .type = type,
        .preconditions =
          {
            {
              Condition_t::HasColonist,
              ConditionCompare::Equals,
              {.boolean = true},
            },
          },
        .effects = {
          {Condition_t::ColonistOnUnclaimedProvince, {.boolean = true}},
        },
      };
    case Action_t::MoveColonistToUnsettledOwnedProvince:
      return Action{
        .type = type,
        .preconditions =
          {
            {
              Condition_t::HasColonist,
              ConditionCompare::Equals,
              {.boolean = true},
            },
            {
              Condition_t::HasUnsettledProvince,
              ConditionCompare::Equals,
              {.boolean = true},
            },
          },
        .effects = {
          {Condition_t::ColonistOnOwnProvince, {.boolean = true}},
        },
      };
  }
};


inline std::vector<Action_t> actions_that_satisfy_cond(Condition_t cond) {
  switch (cond) {
    case Condition_t::ColonistOnUnclaimedProvince:
      return {Action_t::MoveColonistToUnclaimedProvince};
    case Condition_t::ColonistOnOwnProvince:
      return {Action_t::MoveColonistToUnsettledOwnedProvince};
    case Condition_t::HasColonist:
      return {Action_t::SpawnColonist};
    case Condition_t::HasUnsettledProvince:
      return {Action_t::ClaimProvince};
    case Condition_t::HasSettlements:
      return {Action_t::BuildSettlement};
    case Condition_t::COUNT:
      return {};
  }
}

}// namespace AI
