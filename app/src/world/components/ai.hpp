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
    MoveColonistToUnclaimedProvince,
    SpawnColonist,
    ClaimProvince,
    BuildSettlement,
  };
  enum class Condition {
    None,
    ColonistOnUnclaimedProvince,
    ColonistOnOwnProvince,
    HasColonist,
    HasProvince,
    HasSettlement,
  };

  struct Goal {
    Goal_t goal = Goal_t::None;
    Condition desired_state;
  };

  struct Action {
    Action_t type;
    u32 cost = 0;

    list<Condition> preconditions;
    list<Condition> effects;
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
          .cost = 0,
          .preconditions =
            {
              Condition::HasProvince,
            },
          .effects = { Condition::HasSettlement },
        };
      case Action_t::ClaimProvince:
        return Action{
          .type = type,
          .cost = 0,
          .preconditions = { Condition::HasColonist },
          .effects = { Condition::HasProvince },
        };
      case Action_t::SpawnColonist:
        return Action{
          .type = type,
          .cost = 0,
          // @todo requirements to make colonist
          .preconditions{},
          .effects = { Condition::HasColonist },
        };

      case Action_t::MoveColonistToUnclaimedProvince:
        return Action{
          .type = type,
          .cost = 0,
          .preconditions = { Condition::HasColonist },
          .effects = { Condition::ColonistOnUnclaimedProvince },
        };
    }
  };

  inline Goal goal( Goal_t goal_t ) {
    switch ( goal_t ) {
      case Goal_t::None:
        return Goal{
          .goal = Goal_t::None,
          .desired_state = {},
        };
      case Goal_t::EstablishSettlement:
        return Goal{
          .goal = Goal_t::EstablishSettlement,
          .desired_state = Condition::HasSettlement,
        };
    }
  }


};// namespace AI
