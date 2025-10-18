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

enum class Goal_t {
  // BoostProduction,
  // DevelopSettlements,
  None,
  EstablishSettlement,
  ExpandBorders,
};

// @todo actually use
enum class GoalLayer {
  Strategic,
  Operational,
  Tactical,
};

struct Goal {
  Goal_t type;
  GoalLayer layer;
  f32 prio;

  list<Condition> conditions;
};

inline Goal goal(Goal_t goal) {
  switch (goal) {
    case Goal_t::None:
      return Goal{
        .type = Goal_t::None,
        .layer = GoalLayer::Tactical,
        .prio = 0,
        .conditions = {},
      };
    case Goal_t::EstablishSettlement:
      return Goal{
        .type = Goal_t::EstablishSettlement,
        .layer = GoalLayer::Tactical,
        .prio = 1,
        .conditions = {
          Condition{
            Condition_t::HasSettlements,
            ConditionCompare::GreaterThanOrEqualTo,
            {.number = 1},
          },
        }
      };
    case Goal_t::ExpandBorders:
      return Goal{
        .type = Goal_t::ExpandBorders,
        .layer = GoalLayer::Tactical,// @todo
        .prio = 0.5,
        .conditions = {
          Condition{
            Condition_t::HasSettlements,
            ConditionCompare::GreaterThanOrEqualTo,
            {.number = 4},
          },
        }
      };
  }
}

}// namespace AI
