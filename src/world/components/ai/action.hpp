#pragma once

#include "../../../shared/common.hpp"
#include "condition.hpp"

namespace AI {
enum class ActionType {
  AchieveGoal,
  MoveColonistToUnclaimedProvince,
  MoveColonistToUnsettledOwnedProvince,
  SpawnColonist,
  SpawnArmy,
  ClaimProvince,
  BuildSettlement,
  BuildBuilding,
};

struct Action {
  ActionType type;

  float cost = 0;

  list<Condition> preconditions;
  list<Effect> effects;

  str as_str() {
    switch (type) {
      case ActionType::AchieveGoal:
        return "AchieveGoal";
      case ActionType::MoveColonistToUnclaimedProvince:
        return "MoveColonistToUnclaimedProvince";
      case ActionType::MoveColonistToUnsettledOwnedProvince:
        return "MoveColonistToOwnProvince";
      case ActionType::SpawnColonist:
        return "SpawnColonist";
      case ActionType::SpawnArmy:
        return "SpawnArmy";
      case ActionType::ClaimProvince:
        return "ClaimProvince";
      case ActionType::BuildSettlement:
        return "BuildSettlement";
      case ActionType::BuildBuilding:
        return "BuildBuilding";
    }
  }
};

inline Action get_action(ActionType type) {
  switch (type) {
    case ActionType::AchieveGoal:
      return Action{
        .type = type,
        .preconditions = {},
        .effects = {},
      };
    case ActionType::BuildSettlement:
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
    case AI::ActionType::BuildBuilding:
      return Action {
        .type = type,
        .preconditions = {
          {
            ConditionType::HasSettlements, 
            ConditionCompare::GreaterThanOrEqualTo, 
            (u32) 1,
          },
        },
        .effects = {
          
        },
      };
    case ActionType::ClaimProvince:
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
    case ActionType::SpawnColonist:
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

    case ActionType::SpawnArmy:
      return Action{
        .type = type,
        .preconditions =
          {
            {
              ConditionType::HasResources,
              ConditionCompare::GreaterThanOrEqualTo,
              map<Resources::Type, u32>{
                {Resources::Type::Seabass, 10},
              },
            },
          },// @todo requirements to make army
        .effects = {
          Effect{
            ConditionType::HasArmies,
            EffectOperator::Increase,
            (u32) 1,
          },
        },
      };

    case ActionType::MoveColonistToUnclaimedProvince:
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
    case ActionType::MoveColonistToUnsettledOwnedProvince:
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


inline std::vector<ActionType> actions_that_satisfy_cond(ConditionType cond) {
  switch (cond) {
    case ConditionType::ColonistOnUnclaimedProvince:
      return {ActionType::MoveColonistToUnclaimedProvince};
    case ConditionType::ColonistOnOwnProvince:
      return {ActionType::MoveColonistToUnsettledOwnedProvince};
    case ConditionType::HasColonist:
      return {ActionType::SpawnColonist};
    case ConditionType::HasArmies:
      return {ActionType::SpawnArmy};
    case ConditionType::HasUnsettledProvince:
      return {ActionType::ClaimProvince};
    case ConditionType::HasSettlements:
      return {ActionType::BuildSettlement};
    case ConditionType::HasResources:
      return {};// @todo
    case ConditionType::COUNT:
      return {};
  }
}

}// namespace AI
