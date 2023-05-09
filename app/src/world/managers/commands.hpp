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

namespace Commands
{

  enum class Type
  {
    TimeChange,
    Spawn,
    Selection,
    Move,
  };

  struct Command
  {
    Type type;

    std::string msg;
    Vector2 click_pos;
  };


  class Queue : entt::dispatcher
  {
public:
    void FireAll()
    {
      this->update();
    }

    void Enqueue( Command cmd )
    {
      this->enqueue( cmd );
    }

    Queue()
    {
      this->sink<Command>().connect<&Queue::Receive>( this );
    }

private:
    void Receive( const Command &cmd )
    {
      switch ( cmd.type )
      {
        case Type::TimeChange:
        {
          HandleTimeChangeRequest( cmd );
          return;
        }
        case Type::Spawn:
        {
          HandleSpawnRequest( cmd );
          return;
        }
        case Type::Selection:
        {
          SelectionSystem::UpdateSelection( cmd.click_pos );
          return;
        }
        case Type::Move:
        {
          MovementSystem::SetDestinations( Global::state.camera );
          return;
        }
      }
    }


    void HandleTimeChangeRequest( const Command &cmd )
    {
      if ( cmd.msg == "Player request Pause" )
      {
        std::cout << cmd.msg << std::endl;

        if ( Global::state.timeScale > 0.0f )
        {
          Global::state.prevTimeScale = Global::state.timeScale;
          Global::state.timeScale = 0.0f;
        }
        else if ( Global::state.timeScale == 0.0f )
        {
          Global::state.timeScale = Global::state.prevTimeScale;
        }

        return;
      }

      if ( cmd.msg == "Player request Slower" )
      {
        Global::state.timeScale -= 0.5f;
        if ( Global::state.timeScale < 0.0f )
          Global::state.timeScale = 0.0f;

        if ( Global::state.timeScale == 0.0f && Global::state.prevTimeScale > 0.5f )
        {
          Global::state.prevTimeScale -= 0.5f;
          Global::state.timeScale = Global::state.prevTimeScale;
        }

        return;
      }

      if ( cmd.msg == "Player request Faster" )
      {
        Global::state.timeScale += 0.5f;
        if ( Global::state.timeScale > 1.5f )
          Global::state.timeScale = 1.5f;

        return;
      }
    }

    void HandleSpawnRequest( const Command &cmd )
    {

      if ( cmd.msg == "Player spawn Villager" )
      {
        ActorSystem::SpawnColonist( Global::host_player, cmd.click_pos );
        return;
      }

      if ( cmd.msg == "Player spawn City" )
      {
        ProvinceSystem::AssignProvince( Global::host_player, cmd.click_pos );
      }
    }
  };


};// namespace Commands
