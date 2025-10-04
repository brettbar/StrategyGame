#pragma once

#include "vector"
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

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
  // DevelopSettlements,
  None,
  EstablishSettlement,
  ExpandBorders,
};
enum class Action_t {
  AchieveGoal,
  MoveColonistToUnclaimedProvince,
  MoveColonistToUnsettledOwnedProvince,
  SpawnColonist,
  ClaimProvince,
  BuildSettlement,
};
enum class Condition_t {
  ColonistOnUnclaimedProvince,
  ColonistOnOwnProvince,
  HasColonist,
  HasUnsettledProvince,
  HasSettlements,
  COUNT,
};

enum class ConditionValue_t {
  Boolean,
  Number,
};

using ConditionValue = union {
  bool boolean;
  uint32_t number;
};

enum class ConditionCompare {
  Equals,
  GreaterThanOrEqualTo
};

inline ConditionValue_t value_type_for_cond_t(Condition_t cond_t) {
  switch (cond_t) {
    case Condition_t::ColonistOnUnclaimedProvince:
    case Condition_t::ColonistOnOwnProvince:
    case Condition_t::HasColonist:
    case Condition_t::HasUnsettledProvince:
      return ConditionValue_t::Boolean;
    case Condition_t::HasSettlements:
      return ConditionValue_t::Number;
    case Condition_t::COUNT:
      return ConditionValue_t::Boolean;
  }
}


struct Condition {
  Condition_t type;
  ConditionCompare compare = ConditionCompare::Equals;
  ConditionValue value;


  bool Met(Condition against) {

    switch (value_type_for_cond_t(against.type)) {
      case AI::ConditionValue_t::Boolean:
        return value.boolean == against.value.boolean;
      case AI::ConditionValue_t::Number: {
        switch (compare) {
          case AI::ConditionCompare::Equals:
            return value.number == against.value.number;
          case AI::ConditionCompare::GreaterThanOrEqualTo:
            // @TODO are these backwards?
            return value.number >= against.value.number;
        }

      } break;
    }

    return false;
  }

  std::string as_str() {
    switch (type) {
      case Condition_t::ColonistOnUnclaimedProvince:
        return "ColonistOnUnclaimedProvince";
      case Condition_t::ColonistOnOwnProvince:
        return "ColonistOnOwnProvince";
      case Condition_t::HasColonist:
        return "HasColonist";
      case Condition_t::HasUnsettledProvince:
        return "HasUnsettledProvince";
      case Condition_t::HasSettlements:
        return "HasSettlements";
      case Condition_t::COUNT:
        return "";
    }
  }

  // idk why i need this
  bool operator<(const Condition &rhs) const noexcept {
    return this->type < rhs.type;
  }
};

struct Effect {
  Condition_t type;
  ConditionValue value;
};

using WorldState = std::unordered_map<Condition_t, Condition>;

struct Action {
  Action_t type;

  float cost = 0;

  std::vector<Condition> preconditions;
  std::vector<Effect> effects;

  std::string as_str() {
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


// Common type for both Goals and Actions
struct Node {
  Action action;
  std::unordered_map<Condition_t, std::vector<std::shared_ptr<Node>>> children;

  void print(uint32_t depth = 0) {
    for (uint32_t i = 0; i < depth; ++i) {
      std::cout << "  ";
    }
    std::cout << "|--" << action.as_str() << '\n';

    for (const auto &condition: children) {
      for (const auto &child: condition.second) {
        child->print(depth + 1);
      }
    }
  }
};

struct Plan {
  std::vector<Action> stack;
  uint32_t cost;

  void push(Action action) {
    stack.push_back(action);
  }

  Action peek() {
    return stack.back();
  }

  void pop() {
    stack.pop_back();
  }

  void print() {
    for (Action action: stack) {
      printf("%s\n", action.as_str().c_str());
    }
  }
};

inline std::vector<Condition> goal_state(Goal goal) {
  switch (goal) {
    case Goal::None:
      return {};
    case Goal::EstablishSettlement:
      return {
        {
          Condition_t::HasSettlements,
          ConditionCompare::Equals,
          {.number = 1},
        },
      };
    case Goal::ExpandBorders:
      return {
        {
          Condition_t::HasSettlements,
          ConditionCompare::Equals,
          {.number = 3},
        },
      };// @todo
  }
}

inline float default_goal_prio(Goal goal) {
  switch (goal) {
    case Goal::None:
      return 0;
    case Goal::EstablishSettlement:
      return 1;
    case Goal::ExpandBorders:
      return 0.5;
  }
}


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
