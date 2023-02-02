/*
 * Messages are sent to the interface to notify of required updates
 */
#pragma once

#include "../../shared/common.hpp"

namespace Messages {

  namespace {
    struct Message {
      std::string msg;
    };

  };// namespace

  class IManager {
    IManager() {}
    ~IManager() {}

    static IManager *Manager() {
      static IManager instance;
      return &instance;
    }

    IManager( IManager const & ) = delete;
    void operator=( const IManager & ) = delete;
  };
};// namespace Messages
