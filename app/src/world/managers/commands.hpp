/*
   * Commands are essentially requests to modify the game state.
 */
#pragma once

#include "../../shared/common.hpp"
#include "../../shared/global.hpp"
#include "../systems/actor_system.hpp"
#include "../systems/movement_system.hpp"
#include "../systems/province_system.hpp"
#include "../systems/selection_system.hpp"

namespace Commands {

  enum class Type {
    TimeChange,
    Spawn,
    Selection,
    Move,
  };

  namespace {
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

    class IManager {
  public:
      void PostCommand( Type type, const char *msg ) {
        _queue.Enqueue( { type, msg } );
      }

      void PostCommand( Type type, const char *msg, Vector2 pos ) {
        _queue.Enqueue( { type, msg, pos } );
      }

      void FireAll() {
        _queue.update();
      }

      static IManager *Manager() {
        static IManager instance;

        return &instance;
      }

      IManager( IManager const & ) = delete;
      void operator=( const IManager & ) = delete;

  private:
      IManager() {
        this->Listen();
      }

      Queue _queue;

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

      void Listen() {
        _queue.sink<Command>().connect<&IManager::Receive>( this );

        // dispatcher.sink<another_event>().connect<&Listener::method>( listener );
      }


      void HandleTimeChangeRequest( const Command &cmd ) {
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

      void HandleSpawnRequest( const Command &cmd ) {

        if ( cmd.msg == "Player spawn Villager" ) {
          ActorSystem::SpawnColonist( Global::host_player, cmd.click_pos );
          return;
        }

        if ( cmd.msg == "Player spawn City" ) {
          ProvinceSystem::AssignProvince( Global::host_player, cmd.click_pos );
        }
      }
    };

  }// namespace


  // This is not unused, ignore clangd
  inline IManager *Manager() {
    return IManager::Manager();
  }

};// namespace Commands
