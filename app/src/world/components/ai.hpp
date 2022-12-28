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

// Long term goal
enum class Goal {
  BoostProduction,
  ExpandBorders,
  DevelopSettlements,
};

// Short term goal
// to achieve long term goal
enum class Objective {

};

// The approach to achieve
// the Long term goal
enum class Strategy {
  Expand,
};

struct Component {
  Goal current_goal;
  bool has_colonist = false;
  bool has_settlement = false;
};

};// namespace AI
