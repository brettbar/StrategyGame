#pragma once

#include "../shared/common.hpp"
#include <steamclientpublic.h>


// Used for sending data from the Game State to the UI
namespace InterfaceUpdate
{

  /*
     Time to figure out this signal business.
     For InterfaceUpdates we have 2 ideas.

InterfaceUpdates:
     1. can be intended for one to many elements
     2. can be intended for one element only


     1. can update enabled
     2. can update text 
     3. can update background
     4. can update clickable
 */

  inline entt::dispatcher dispatcher{};

  enum class Type : u32
  {
    INVALID,
    EnabledUpdate,
    TextUpdate,
    BackgroundUpdate,
    ClickableUpdate,

    NumTypes,
  };

  enum ID
  {
    INVALID,
    FactionSelected,
    SettlementContext,
    ActorContext,
    JoinLobby,
    HostLobby,
    NumIDs,
    PlayerToggledReady,
  };

  struct Data
  {
    Type type = Type::INVALID;
    ID update_id = ID::INVALID;
    bool on = false;
    std::string updated_text = "INVALID";
    Color updated_background = BLACK;
    bool clickable = false;

    bool targeted = false;
    std::string target = "INVALID";

    void send()
    {
      // TODO(??)
      // dispatcher.enqueue( this );
      dispatcher.enqueue( *this );
    }
  };

  struct Builder
  {
protected:
    Data _data;

public:
    Data build()
    {
      return _data;
    };
  };

  struct EnabledUpdate : Builder
  {
    EnabledUpdate( ID update_id, bool on )
    {
      _data.type = Type::EnabledUpdate;
      _data.update_id = update_id;
      _data.on = on;
    }

    EnabledUpdate &SetTarget( std::string target_id )
    {
      _data.targeted = true;
      _data.target = target_id;
      return *this;
    }
  };

  struct Text : Builder
  {
    Text( ID update_id )
    {
      _data.type = Type::TextUpdate;
      _data.update_id = update_id;
    }

    Text &SetText( std::string updated_text )
    {
      _data.updated_text = updated_text;
      return *this;
    }

    Text &SetTarget( std::string target_id )
    {
      _data.targeted = true;
      _data.target = target_id;
      return *this;
    }
  };

  struct Background : Builder
  {
    Background( ID update_id, Color updated_background )
    {
      _data.type = Type::BackgroundUpdate;
      _data.update_id = update_id;
      _data.updated_background = updated_background;
    }

    Background &SetTarget( std::string target_id )
    {
      _data.targeted = true;
      _data.target = target_id;
      return *this;
    }
  };

  struct Clickable : Builder
  {
    Clickable( ID update_id, bool clickable )
    {
      _data.type = Type::ClickableUpdate;
      _data.update_id = update_id;
      _data.clickable = clickable;
    }

    Clickable &SetTarget( std::string target_id )
    {
      _data.targeted = true;
      _data.target = target_id;
      return *this;
    }
  };


};// namespace InterfaceUpdate
