#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"

namespace Commands {

  enum class Type {
    Move,
    TimeChange,
    Spawn,
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
  };

  class System {
public:
    entt::dispatcher queue;

    void init() {
      queue = entt::dispatcher{};
      // queue.sink<Commands::Command>().connect<&System::receive>( this );
    }

    void poll() {
      queue.update();
    }

    // void receive( const Command &cmd ) {}

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