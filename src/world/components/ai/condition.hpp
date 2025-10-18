#pragma once

#include "../../../shared/common.hpp"

namespace AI {
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
  u32 number;
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


  bool operator==(const ConditionValue &other) const {
    switch (value_type_for_cond_t(type)) {
      case ConditionValue_t::Boolean:
        return value.boolean == other.boolean;
      case ConditionValue_t::Number:
        return value.number == other.number;
    }
  }


  bool is_satisfied_by(const ConditionValue &against) {
    switch (compare) {
      case AI::ConditionCompare::Equals:
        return *this == against;
      case AI::ConditionCompare::GreaterThanOrEqualTo: {
        switch (value_type_for_cond_t(type)) {
          case ConditionValue_t::Boolean:
            return false;
          case ConditionValue_t::Number:
            return against.number >= value.number;
        }
      }
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


enum class EffectOperator {
  Set,
  Increase,
  Decrease,
};

struct Effect {
  Condition_t type;
  EffectOperator op;
  ConditionValue value;

  ConditionValue before = {};
  ConditionValue after = {};
};


}// namespace AI
