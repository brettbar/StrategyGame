#pragma once

#include "common.hpp"
#include "global.hpp"
#include <iostream>

namespace Commands {

enum class Type {
  TimeChange,
};

struct Command {
  // std::string requester;
  Type type;

  std::string msg;
};

struct Queue : entt::dispatcher {};

inline void HandleTimeChangeRequest( const Command & );

// TODO figure out how to execute command that
// was called with enqueue
struct Listener {

  void receive( const Command &cmd ) {

    switch ( cmd.type ) {
      case Commands::Type::TimeChange: {
        HandleTimeChangeRequest( cmd );
      }
    }

    //
  }


  // void method(const another_event &) { /* ... */ }
};

inline Listener listener;
inline Queue queue;

inline void Listen() {
  queue.sink<Command>().connect<&Listener::receive>( listener );

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


};// namespace Commands
