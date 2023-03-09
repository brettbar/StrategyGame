#pragma once

#include "../../shared/common.hpp"

#include "../ui_system.hpp"

namespace UI
{

  inline std::vector<Element> CreateFactionButtons()
  {
    // TODO replace with read in from json
    std::map<std::string, Color> factions = {
      { "romans", RED },
      { "greeks", BLUE },
      { "celts", GREEN },
      { "punics", PURPLE },
      { "persians", ORANGE },
      { "scythians", PINK },
      { "germans", GRAY },
    };
    // TODO replace with read in from json
    std::map<std::string, std::string> full_names = {
      { "romans", "Roman Republic" },
      { "greeks", "Greek Cities" },
      { "celts", "Celtic Tribes" },
      { "punics", "Punic Colonies" },
      { "persians", "Persian Empire" },
      { "scythians", "Scythian Horde" },
      { "germans", "Germanic Federation" },
    };

    std::vector<Element> text_buttons = {
      TextLabel( "faction_select_label" ).SetText( "Select your faction", 32 ),
    };

    for ( const auto &[name, color]: factions )
    {
      std::string faction = name;
      std::string id = "faction_select_" + faction;

      text_buttons.push_back( TextButton( id )
                                .SetText( full_names[name], 32 )
                                .Background( color )
                                .SetEvent( InterfaceEvent::Data(
                                  InterfaceEvent::ID::FactionSelected, faction
                                ) ) );
    }

    return text_buttons;
  }

  inline std::vector<Element> CreateFactionSelectMenuUI()
  {
    return {
      Panel( "faction_select_menu" )
        .SetAxis( Axis::Column )
        .SetAnchor( Anchor::Centered )
        .Children( CreateFactionButtons() ),
    };
  }


};// namespace UI
