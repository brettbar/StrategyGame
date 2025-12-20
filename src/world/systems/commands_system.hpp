#pragma once

#include "../../../data/buildings.hpp"
#include "../../shared/common.hpp"
#include <optional>

namespace Commands {

enum class Type {
  None,
  Move,
  TimeChange,
  SpawnColonist,
  SpawnArmy,
  ClaimProvince,
  BuildSettlement,
  ConstructBuilding,
};

struct Command {
  Type type;

  // bool checked = false;
  bool can_execute = false;


  entt::entity player_e;
  str msg;
  Vector2 click_pos;
  Buildings::Building building;

  entt::entity entity;

  static Command none() {
    auto cmd = Command();
    cmd.type = Type::None;
    return cmd;
  }

  static Command move(
    entt::entity player,
    vec2f click_pos,
    entt::entity selected_e
  ) {
    auto cmd = Command();
    cmd.type = Type::Move;
    cmd.player_e = player;
    cmd.click_pos = click_pos;
    cmd.entity = selected_e;
    return cmd;
  }


  static Command build_settlement(entt::entity entity) {
    auto cmd = Command();
    cmd.type = Type::BuildSettlement;
    cmd.entity = entity;
    return cmd;
  }

  static Command claim_province(entt::entity entity) {
    auto cmd = Command();
    cmd.type = Type::ClaimProvince;
    cmd.entity = entity;
    return cmd;
  }

  static Command spawn_colonist(entt::entity player, vec2f click_pos) {
    auto cmd = Command();
    cmd.type = Type::SpawnColonist;
    cmd.player_e = player;
    cmd.click_pos = click_pos;
    return cmd;
  }

  static Command spawn_army(entt::entity player, vec2f click_pos) {
    auto cmd = Command();
    cmd.type = Type::SpawnArmy;
    cmd.player_e = player;
    cmd.click_pos = click_pos;
    return cmd;
  }

  static Command time_change(entt::entity player, str msg) {
    auto cmd = Command();
    cmd.type = Type::TimeChange;
    cmd.player_e = player;
    cmd.msg = msg;
    return cmd;
  }

  static Command construct_building(
    entt::entity settlement_e,
    Buildings::Building building
  ) {
    auto cmd = Command();
    cmd.type = Type::ConstructBuilding;
    cmd.entity = settlement_e;
    cmd.building = building;
    return cmd;
  }

  // private:
  //     Command() {}
};

class System {
  public:
  // entt::dispatcher queue;
  list<Commands::Command> queue;

  void init() {
    // queue = entt::dispatcher{};
    queue = {};
  }

  void clear() {
    queue.clear();
  }

  // void poll() {
  //   queue.update();
  // }

  bool is_empty() {
    return queue.empty();
  }

  void enqueue(const Command &cmd) {
    queue.push_back(cmd);
  }

  opt<Commands::Command> pop() {
    if (!queue.empty()) {
      Commands::Command last_val = queue.back();
      queue.pop_back();
      return last_val;
    }

    return {};
  }

  static System *Singleton() {
    static System instance;
    return &instance;
  }

  private:
  System() {}
  ~System() {}

  System(System const &) = delete;
  void operator=(const System &) = delete;
};

inline System *Manager() {
  return System::Singleton();
}
};// namespace Commands
