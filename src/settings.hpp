#pragma once
#include "shared/common.hpp"
#include "ui/common.h"
#include <raylib.h>


namespace Settings {
  enum class WindowState {
    Fullscreen,
    Borderless,
    Window,
  };

  // @todo make this serializable so we can load and save a settings file
  struct State {
    WindowState window_state;
    vec2i resolution;
    u32 fps_limit;
  };

  struct IManager {
    State _state;

    static IManager *Manager() {
      static IManager instance;
      return &instance;
    }

    IManager() {
      _state = State{
        .window_state = WindowState::Fullscreen,
        .resolution = {GetScreenWidth(), GetScreenHeight()},
        .fps_limit = 300,
      };
    }

    // @todo we can use this for reading from file or something
    IManager(State state) {
      _state = state;
    }

    vec2i resolution() {
      return _state.resolution;
    }

    void limit_fps(u32 limit) {
      _state.fps_limit = limit;
      SetTargetFPS(_state.fps_limit);
    }

    void set_window_state(WindowState window_state) {
      _state.window_state = window_state;

      switch (window_state) {
        case WindowState::Fullscreen: {
          SetConfigFlags(FLAG_FULLSCREEN_MODE);
        } break;
        case WindowState::Borderless: {
          SetConfigFlags(FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED);
        } break;
        case WindowState::Window: {
          SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        } break;
      }
    }
  };

  inline IManager *Manager() {
    return IManager::Manager();
  }
}// namespace Settings
