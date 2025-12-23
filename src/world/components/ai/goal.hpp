#pragma once

#include "../../../shared/common.hpp"
#include "condition.hpp"

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

enum class GoalType {
  // BoostProduction,
  // DevelopSettlements,
  None,
  EstablishSettlement,
  BuildArmy,
  ExpandBorders,
};

// @todo actually use
enum class GoalLayer {
  Strategic,
  Operational,
  Tactical,
};

struct Goal {
  GoalType type;
  GoalLayer layer;
  f32 prio;

  list<Condition> conditions;

  std::string as_str() {
    switch (type) {
      case GoalType::None:
        return "None";
      case GoalType::EstablishSettlement:
        return "EstablishSettlement";
      case GoalType::BuildArmy:
        return "BuildArmy";
      case GoalType::ExpandBorders:
        return "ExpandBorders";
    }
  }
};

inline Goal goal(GoalType goal) {
  switch (goal) {
    case GoalType::None:
      return Goal{
        .type = GoalType::None,
        .layer = GoalLayer::Tactical,
        .prio = 0,
        .conditions = {},
      };
    case GoalType::EstablishSettlement:
      return Goal{
        .type = GoalType::EstablishSettlement,
        .layer = GoalLayer::Tactical,
        .prio = 1,
        .conditions = {
          Condition{
            ConditionType::HasSettlements,
            ConditionCompare::GreaterThanOrEqualTo,
            (u32) 1,
          },
        }
      };
    case GoalType::BuildArmy:
      return Goal{
        .type = GoalType::BuildArmy,
        .layer = GoalLayer::Tactical,
        .prio = 0.75,
        .conditions = {
          Condition{
            ConditionType::HasArmies,
            ConditionCompare::GreaterThanOrEqualTo,
            (u32) 1,
          },
        }
      };
    case GoalType::ExpandBorders:
      return Goal{
        .type = GoalType::ExpandBorders,
        .layer = GoalLayer::Tactical,// @todo
        .prio = 0.5,
        .conditions = {
          Condition{
            ConditionType::HasSettlements,
            ConditionCompare::GreaterThanOrEqualTo,
            (u32) 3,
          },
        }
      };
  }
}

}// namespace AI
