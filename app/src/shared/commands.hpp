// Thinking commands should only matter to the game running,
// thus could go in the normal start/load sequence
// Stuff like ui menu button presses dont need to be commands,
// They should be their own thing that doesnt relate to the game simulation

#pragma once

#include "../world/systems/province_system.hpp"
#include "../world/systems/spawn_system.hpp"
#include "common.hpp"
#include "global.hpp"
#include <iostream>

namespace Commands {

enum class Type {
  TimeChange,
  Spawn,
  Selection,
  Move,
};

struct Command {
  Type type;

  std::string msg;
  Vector2 click_pos;
};

struct Queue : entt::dispatcher {
  void Enqueue( const Command &cmd ) {
    // Send the command to all other players

    this->enqueue( cmd );
  }
};

inline void HandleTimeChangeRequest( const Command & );
inline void HandleSpawnRequest( const Command & );

struct Listener {
  void Receive( const Command &cmd ) {
    switch ( cmd.type ) {
      case Type::TimeChange: {
        HandleTimeChangeRequest( cmd );
        return;
      }
      case Type::Spawn: {
        HandleSpawnRequest( cmd );
        return;
      }
      case Type::Selection: {
        SelectionSystem::UpdateSelection( cmd.click_pos );
        return;
      }
      case Type::Move: {
        MovementSystem::SetDestinations( Global::state.camera );
        return;
      }
    }
  }


  // void method(const another_event &) { /* ... */ }
};

inline Listener listener;
inline Queue queue;


inline void Listen() {
  queue.sink<Command>().connect<&Listener::Receive>( listener );

  // dispatcher.sink<another_event>().connect<&Listener::method>( listener );
}

inline void FireAll() {
  queue.update();
}


inline void HandleTimeChangeRequest( const Command &cmd ) {
  if ( cmd.msg == "Player request Pause" ) {
    std::cout << cmd.msg << std::endl;

    if ( Global::state.timeScale > 0.0f ) {
      Global::state.prevTimeScale = Global::state.timeScale;
      Global::state.timeScale = 0.0f;
    }
    else if ( Global::state.timeScale == 0.0f ) {
      Global::state.timeScale = Global::state.prevTimeScale;
    }

    return;
  }

  if ( cmd.msg == "Player request Slower" ) {
    Global::state.timeScale -= 0.5f;
    if ( Global::state.timeScale < 0.0f )
      Global::state.timeScale = 0.0f;

    if ( Global::state.timeScale == 0.0f && Global::state.prevTimeScale > 0.5f ) {
      Global::state.prevTimeScale -= 0.5f;
      Global::state.timeScale = Global::state.prevTimeScale;
    }

    return;
  }

  if ( cmd.msg == "Player request Faster" ) {
    Global::state.timeScale += 0.5f;
    if ( Global::state.timeScale > 1.5f )
      Global::state.timeScale = 1.5f;

    return;
  }
}

inline void HandleSpawnRequest( const Command &cmd ) {

  if ( cmd.msg == "Player spawn Villager" ) {
    SpawnSystem::SpawnColonist( Global::host_player, cmd.click_pos );
    return;
  }

  if ( cmd.msg == "Player spawn City" ) {
    ProvinceSystem::AssignProvince( Global::host_player, cmd.click_pos );
  }
}


};// namespace Commands
