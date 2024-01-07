#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"

namespace Commands {

  enum class Type {
    Move,
    TimeChange,
    SpawnActor,
    ClaimProvince,
    BuildSettlement,
  };

  struct Command {
    Type type;
    // bool checked = false;
    // bool can_execute = false;


    entt::entity player_e;
    str msg;
    Vector2 click_pos;

    entt::entity entity;


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


    static Command build_settlement( entt::entity entity ) {
      auto cmd = Command();
      cmd.type = Type::BuildSettlement;
      cmd.entity = entity;
      return cmd;
    }

    static Command claim_province( entt::entity entity ) {
      auto cmd = Command();
      cmd.type = Type::ClaimProvince;
      cmd.entity = entity;
      return cmd;
    }

    static Command spawn_actor( entt::entity player, vec2f click_pos ) {
      auto cmd = Command();
      cmd.type = Type::SpawnActor;
      cmd.player_e = player;
      cmd.click_pos = click_pos;
      return cmd;
    }

    static Command time_change( entt::entity player, str msg ) {
      auto cmd = Command();
      cmd.type = Type::TimeChange;
      cmd.player_e = player;
      cmd.msg = msg;
      return cmd;
    }

private:
    Command() {}
  };

  class System {
public:
    entt::dispatcher queue;

    void init() {
      queue = entt::dispatcher{};
    }

    void poll() {
      queue.update();
    }

    void enqueue( const Command &cmd ) {
      queue.enqueue( cmd );
    }

    static System *Singleton() {
      static System instance;
      return &instance;
    }

private:
    System() {}
    ~System() {}

    System( System const & ) = delete;
    void operator=( const System & ) = delete;
  };

  inline System *Manager() {
    return System::Singleton();
  }
};// namespace Commands
