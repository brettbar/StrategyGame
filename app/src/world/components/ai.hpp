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
    DoNothing,
    MoveUnit,
    SpawnColonist,
    ClaimProvince,
    BuildSettlement,
  };
  enum class Condition {
    None,
    HasColonistOnEligibleTerrain,
    HasProvince,
    HasSettlement,
  };

  struct Goal {
    Goal_t goal = Goal_t::None;
    list<Condition> desired_state;
  };

  struct Action {
    Action_t type;
    f32 cost;

    list<Condition> preconditions;
    // @note might need to be plural in the future
    Condition effect;
  };

  struct Plan {
    list<Action> stack;

    void push( Action action ) {
      stack.push_back( action );
    }

    Action peek() {
      return stack.back();
    }

    void pop() {
      stack.pop_back();
    }
  };


  struct Component {
    Goal current_goal;
    Plan current_plan;
    bool executing_plan = false;
    // bool has_colonist = false;
    // bool has_settlement = false;
  };

};// namespace AI
