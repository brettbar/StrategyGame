#pragma once

#include "../../../../data/resources.hpp"
#include "../../../shared/common.hpp"
#include <variant>

namespace AI {
enum class ConditionType {
  ColonistOnUnclaimedProvince,
  ColonistOnOwnProvince,
  HasColonist,
  HasUnsettledProvince,
  HasSettlements,
  HasResources,
  HasArmies,
  COUNT,
};

enum class ConditionValueType {
  Boolean,
  Number,
  Resources,
};

using ConditionValue = std::variant<bool, u32, map<Resources::Type, u32>>;

enum class ConditionCompare {
  Equals,
  GreaterThanOrEqualTo
};

inline ConditionValueType value_type_for_cond_t(ConditionType cond_t) {
  switch (cond_t) {
    case ConditionType::ColonistOnUnclaimedProvince:
    case ConditionType::ColonistOnOwnProvince:
    case ConditionType::HasColonist:
    case ConditionType::HasUnsettledProvince:
      return ConditionValueType::Boolean;
    case ConditionType::HasSettlements:
    case ConditionType::HasArmies:
      return ConditionValueType::Number;
    case ConditionType::HasResources:
      return ConditionValueType::Resources;
    case ConditionType::COUNT:
      return ConditionValueType::Boolean;
  }
}

struct Condition {
  ConditionType type;
  ConditionCompare compare = ConditionCompare::Equals;
  ConditionValue value = {};

  Condition(ConditionType cond_t) : type(cond_t), value({}) {}

  Condition(ConditionType cond_t, ConditionCompare comp, ConditionValue cond_v)
      : type(cond_t), compare(comp), value(cond_v) {}

  Condition(ConditionType cond_t, ConditionValue cond_v)
      : type(cond_t), value(cond_v) {}

  bool operator==(const ConditionValue &other) const {
    switch (value_type_for_cond_t(type)) {
      case ConditionValueType::Boolean:
        return std::get<bool>(value) == std::get<bool>(other);
      case ConditionValueType::Number:
        return std::get<u32>(value) == std::get<u32>(other);
      case ConditionValueType::Resources:
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
          case ConditionValueType::Boolean:
            return false;
          case ConditionValueType::Number:
            return std::get<u32>(against) >= std::get<u32>(value);
          case ConditionValueType::Resources: {
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
      case ConditionType::ColonistOnUnclaimedProvince:
        return "ColonistOnUnclaimedProvince";
      case ConditionType::ColonistOnOwnProvince:
        return "ColonistOnOwnProvince";
      case ConditionType::HasColonist:
        return "HasColonist";
      case ConditionType::HasUnsettledProvince:
        return "HasUnsettledProvince";
      case ConditionType::HasSettlements:
        return "HasSettlements";
      case ConditionType::HasResources:
        return "HasResources";
      case ConditionType::HasArmies:
        return "HasArmies";
      case ConditionType::COUNT:
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
  ConditionType type;
  EffectOperator op;
  ConditionValue value;

  ConditionValue before = {};
  ConditionValue after = {};
};

inline list<Condition> has_food(u32 quantity) {
  list<Condition> food_conditions = {};

  for (const auto resource: Resources::food_resources) {
    food_conditions.push_back({
      ConditionType::HasResources,
      ConditionCompare::GreaterThanOrEqualTo,
      map<Resources::Type, u32>{{resource, quantity}},
    });
  }

  return food_conditions;
}


}// namespace AI
