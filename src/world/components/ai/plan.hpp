#pragma once

#include "../../../shared/common.hpp"
#include "action.hpp"
#include "condition.hpp"


namespace AI {
// Common type for both Goals and Actions
struct Node {
  Action action;
  map<ConditionType, std::vector<std::shared_ptr<Node>>> children;

  void print(uint32_t depth = 0) {
    for (uint32_t i = 0; i < depth; ++i) {
      printf("  ");
    }
    printf("|-- %s\n", action.as_str().c_str());

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
}// namespace AI
