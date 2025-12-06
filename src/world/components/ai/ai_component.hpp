#pragma once

#include "../../../shared/common.hpp"

#include "goal.hpp"
#include "plan.hpp"


namespace AI {

struct Component {
  Goal_t current_goal;
  Plan current_plan;
  sptr<Action> current_action = nullptr;
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
