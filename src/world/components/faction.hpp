#pragma once

#include "../../shared/common.hpp"

#include "actor.hpp"
#include "cereal/details/helpers.hpp"

namespace Faction {

  enum class Mobility_t {
    Settled,
    Tribal,
    Nomadic
  };

  inline str as_str( Mobility_t type ) {
    switch ( type ) {
      case Mobility_t::Settled:
        return "settled";
      case Mobility_t::Tribal:
        return "tribal";
      case Mobility_t::Nomadic:
        return "nomadic";
    }
  }

  inline Mobility_t from_str( str type_str ) {
    if ( type_str == "settled" )
      return Mobility_t::Settled;
    if ( type_str == "tribal" )
      return Mobility_t::Tribal;
    if ( type_str == "nomadic" )
      return Mobility_t::Nomadic;

    // @todo error handle
    return Mobility_t::Settled;
  }


  enum class Government_t {
    Federation,
    Republic,
    Empire
  };

  inline str gov_as_str( Government_t gov ) {
    switch ( gov ) {
      case Government_t::Empire:
        return "empire";
      case Government_t::Federation:
        return "federation";
      case Government_t::Republic:
        return "republic";
    }
  }

  inline Government_t gov_from_str( str gov_str ) {
    if ( gov_str == "empire" )
      return Government_t::Empire;
    if ( gov_str == "republic" )
      return Government_t::Republic;
    if ( gov_str == "federation" )
      return Government_t::Federation;

    // @todo error handle
    return Government_t::Empire;
  }


  struct Component {
    str id;

    struct Colors {
      str primary;
      str secondary;

      template<class Archive>
      void serialize( Archive &ar ) {
        ar( primary, secondary );
      }
    } colors;

    struct Culture {
      str adjective;
      str denonym;
      list<str> settlement_names;

      template<class Archive>
      void serialize( Archive &ar ) {
        ar( adjective, denonym, settlement_names );
      }
    } culture;

    struct Government {
      Government_t start_gov;
      str empire_name;
      str empire_leader_title;
      str republic_name;
      str republic_leader_title;
      str federation_name;
      str federation_leader_title;

      template<class Archive>
      void serialize( Archive &ar ) {
        ar(
          gov_as_str( start_gov ),
          empire_name,
          empire_leader_title,
          republic_name,
          republic_leader_title,
          federation_name,
          federation_leader_title
        );
      }

    } government;

    struct Mobility {
      Mobility_t type;

      template<class Archive>
      void serialize( Archive &ar ) {
        ar( as_str( type ) );
      }

    } mobility;

    struct Roster {
      map<Actor::Type, Actor::Data> actors;
    } roster;


    template<class Archive>
    void serialize( Archive &ar ) {
      ar( id );
      colors.serialize( ar );
      culture.serialize( ar );
      government.serialize( ar );
      mobility.serialize( ar );
      ar( CEREAL_NVP( roster.actors ) );
    }
  };

};// namespace Faction
