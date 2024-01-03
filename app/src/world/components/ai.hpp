#pragma once

#include "../../shared/common.hpp"


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
    // ExpandBorders,
    // DevelopSettlements,
    None,
    EstablishSettlement,
  };
  enum class Action_t {
    MoveUnit,
    ClaimProvince,
  };
  enum class Condition_t {
    HasSettlement,
  };

  struct Condition {
    Condition_t condition;

    func<bool()> is_satisified;
  };

  struct Goal {
    Goal_t goal = Goal_t::None;
    list<Condition> conditions;

    bool succeeded() {
      bool all_satisfied = false;

      for ( auto cond: conditions )
        all_satisfied = all_satisfied && cond.is_satisified();

      return all_satisfied;
    }
  };

  struct Action {
    Action_t action;
    f32 cost;

    list<Condition> preconditions;
    list<Condition> effects;

    bool ready() {
      bool all_satisfied = false;

      for ( auto cond: preconditions )
        all_satisfied = all_satisfied && cond.is_satisified();

      return all_satisfied;
    }
  };

  struct Plan {
    list<Action> stack;
  };


  struct Component {
    Goal current_goal;
    // bool has_colonist = false;
    // bool has_settlement = false;
  };

};// namespace AI
