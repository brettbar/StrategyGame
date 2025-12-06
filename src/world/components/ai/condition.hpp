#pragma once

#include "../../../../data/resources.hpp"
#include "../../../shared/common.hpp"
#include <variant>

namespace AI {
enum class Condition_t {
  ColonistOnUnclaimedProvince,
  ColonistOnOwnProvince,
  HasColonist,
  HasUnsettledProvince,
  HasSettlements,
  HasResources,
  COUNT,
};

enum class ConditionValue_t {
  Boolean,
  Number,
  Resources,
};

// using ConditionValue = union {
//   bool boolean;
//   u32 number;
//   map<Resources::Type, u32> resources;
// };

using ConditionValue = std::variant<bool, u32, map<Resources::Type, u32>>;

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
    case Condition_t::HasResources:
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
        return std::get<bool>(value) == std::get<bool>(other);
      case ConditionValue_t::Number:
        return std::get<u32>(value) == std::get<u32>(other);
      case ConditionValue_t::Resources:
        return std::get<map<Resources::Type, u32>>(value) ==
               std::get<map<Resources::Type, u32>>(other);
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
            return std::get<u32>(against) >= std::get<u32>(value);
          case ConditionValue_t::Resources:
            // @verify does this actually work?
            return std::get<map<Resources::Type, u32>>(against) >=
                   std::get<map<Resources::Type, u32>>(value);
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
      case Condition_t::HasResources:
        return "HasResources";
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
