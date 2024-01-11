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

  enum class Goal {
    // BoostProduction,
    // ExpandBorders,
    // DevelopSettlements,
    None,
    EstablishSettlement,
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

    list<Condition> preconditions;
    list<Condition> effects;
  };

  struct Node {
    Action action;
    u32 cost = 0;
    list<Node> children;
  };

  struct Plan {
    list<Action> stack;
    u32 cost;

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

  inline Action get_action( Action_t type ) {
    switch ( type ) {
      case Action_t::BuildSettlement:
        return Action{
          .type = type,
          .preconditions =
            {
              Condition::HasProvince,
            },
          .effects = { Condition::HasSettlement },
        };
      case Action_t::ClaimProvince:
        return Action{
          .type = type,
          .preconditions = { Condition::HasColonist },
          .effects = { Condition::HasProvince },
        };
      case Action_t::SpawnColonist:
        return Action{
          .type = type,
          // @todo requirements to make colonist
          .preconditions{},
          .effects = { Condition::HasColonist },
        };

      case Action_t::MoveColonistToUnclaimedProvince:
        return Action{
          .type = type,
          .preconditions = { Condition::HasColonist },
          .effects = { Condition::ColonistOnUnclaimedProvince },
        };
    }
  };

  inline list<Condition> goal_conds( Goal goal ) {
    switch ( goal ) {
      case Goal::None:
        return {};
      case Goal::EstablishSettlement:
        return { Condition::HasSettlement };
    }
  }


};// namespace AI
