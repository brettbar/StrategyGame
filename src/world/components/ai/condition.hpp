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
      return ConditionValue_t::Number;
    case Condition_t::HasResources:
      return ConditionValue_t::Resources;
    case Condition_t::COUNT:
      return ConditionValue_t::Boolean;
  }
}

struct Condition {
  Condition_t type;
  ConditionCompare compare = ConditionCompare::Equals;
  ConditionValue value = {};

  Condition(Condition_t cond_t) : type(cond_t), value({}) {}

  Condition(Condition_t cond_t, ConditionCompare comp, ConditionValue cond_v)
      : type(cond_t), compare(comp), value(cond_v) {}

  Condition(Condition_t cond_t, ConditionValue cond_v)
      : type(cond_t), value(cond_v) {}

  bool operator==(const ConditionValue &other) const {
    switch (value_type_for_cond_t(type)) {
      case ConditionValue_t::Boolean:
        return std::get<bool>(value) == std::get<bool>(other);
      case ConditionValue_t::Number:
        return std::get<u32>(value) == std::get<u32>(other);
      case ConditionValue_t::Resources:
        // @todo wont work
        // we need to change it so its just saying does all of one equal that of the other, not a strict check I think
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
          case ConditionValue_t::Resources: {
            map<Resources::Type, u32> resources_needed =
              std::get<map<Resources::Type, u32>>(value);

            map<Resources::Type, u32> comparing_against =
              std::get<map<Resources::Type, u32>>(against);


            for (const auto &[key, val]: resources_needed) {
              Resources::Type resource_needed = key;
              u32 amount_needed = val;

              u32 amount_currently_have = comparing_against[resource_needed];

              if (amount_currently_have < amount_needed) {
                return false;
              }
            }

            return true;
          } break;
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
