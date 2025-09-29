#pragma once

#include "../../data/ai.hpp"
#include "../../shared/common.hpp"


namespace AI {

// Common type for both Goals and Actions
struct Node {
  Action action;
  map<Condition_t, list<sptr<Node>>> children;

  void print(u32 depth = 0) {
    for (u32 i = 0; i < depth; ++i) {
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
  list<Action> stack;
  u32 cost;

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

struct Component {
  Goal current_goal;
  Plan current_plan;
  bool executing_plan = false;
  // map<AI::Condition_t, ConditionValue> world_state;
  // bool has_colonist = false;
  // bool has_settlement = false;

  template<class Archive>
  void serialize(Archive &ar) {
    ar(current_goal);
  }
};

};// namespace AI
//
//
// for each condition of your goal
//    for i = 0; i < (cond.goal_num - world.current_num); i++
//      add a seperate plan to the plan array
//
//
// for each plan in plan_array
//  execute the plan
