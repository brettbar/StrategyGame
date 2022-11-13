#pragma once

#include "state.hpp"


namespace Events {

// Used for sinking new event types
inline entt::dispatcher dispatcher{};

// Base class which other system's event listeners will inherit from
struct Listener {
  State *currState;

  Listener() { currState = nullptr; }
  ~Listener() {}

  Listener( State &state ) { this->currState = &state; }

  inline void Update( State &state ) { this->currState = &state; }

  inline virtual void Receive() {}
};


// The main listener to receive and process events from various sources
// just as input and ui
struct EventListener : Listener {
  inline void Listen() {}
};

inline EventListener event_listener;

inline void Init() { event_listener.Listen(); }

inline void Update( State &state ) { event_listener.Update( state ); }
};// namespace Events
