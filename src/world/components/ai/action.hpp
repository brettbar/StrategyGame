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
  BuildBuilding,
};

struct Action {
  Action_t type;

  float cost = 0;

  list<Condition> preconditions;
  list<Effect> effects;

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
      case Action_t::BuildBuilding:
        return "BuildBuilding";
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
            {ConditionType::ColonistOnOwnProvince, true},
            {
              ConditionType::HasResources,
              ConditionCompare::GreaterThanOrEqualTo,
              map<Resources::Type, u32>{
                {Resources::Type::Timber, 5},
              },
            },
          },
        .effects = {
          Effect{
            ConditionType::HasSettlements,
            EffectOperator::Increase,
            (u32) 1,
          },
        },
      };
    case AI::Action_t::BuildBuilding:
      return Action {
        .type = type,
        .preconditions = {
          {ConditionType::HasSettlements, true},
        },
        .effects = {
          
        },
      };
    case Action_t::ClaimProvince:
      return Action{
        .type = type,
        .preconditions =
          {
            {ConditionType::ColonistOnUnclaimedProvince, true},
          },
        .effects = {
          Effect{
            ConditionType::HasUnsettledProvince,
            EffectOperator::Set,
            true,
          },
        },
      };
    case Action_t::SpawnColonist:
      return Action{
        .type = type,
        // @todo requirements to make colonist
        .preconditions{},
        .effects = {
          Effect{
            ConditionType::HasColonist,
            EffectOperator::Set,
            true,
          },
        },
      };

    case Action_t::MoveColonistToUnclaimedProvince:
      return Action{
        .type = type,
        .preconditions =
          {
            {ConditionType::HasColonist, true},
          },
        .effects = {
          Effect{
            ConditionType::ColonistOnUnclaimedProvince,
            EffectOperator::Set,
            true,
          },
        },
      };
    case Action_t::MoveColonistToUnsettledOwnedProvince:
      return Action{
        .type = type,
        .preconditions =
          {
            {ConditionType::HasColonist, true},
            {ConditionType::HasUnsettledProvince, true},
          },
        .effects = {
          Effect{
            ConditionType::ColonistOnOwnProvince,
            EffectOperator::Set,
            true,
          },
        },
      };
  }
};


inline std::vector<Action_t> actions_that_satisfy_cond(ConditionType cond) {
  switch (cond) {
    case ConditionType::ColonistOnUnclaimedProvince:
      return {Action_t::MoveColonistToUnclaimedProvince};
    case ConditionType::ColonistOnOwnProvince:
      return {Action_t::MoveColonistToUnsettledOwnedProvince};
    case ConditionType::HasColonist:
      return {Action_t::SpawnColonist};
    case ConditionType::HasUnsettledProvince:
      return {Action_t::ClaimProvince};
    case ConditionType::HasSettlements:
      return {Action_t::BuildSettlement};
    case ConditionType::HasResources:
      return {};
    case ConditionType::COUNT:
      return {};
  }
}

}// namespace AI
