#pragma once

#include "../../shared/common.hpp"

#include "../ui_system.hpp"

namespace UI
{
  inline sptr<Element> CreateFactionSelectMenuUI()
  {

    return GridPanel( "faction_select_menu", 1, 7 );
  }

  inline list<sptr<Element>> CreateFactionButtons()
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

    list<sptr<Element>> text_buttons = {
      TextLabel( "faction_select_label", "Select your faction", 32 ),
    };

    for ( const auto &[name, color]: factions )
    {
      std::string faction = name;
      std::string id = "faction_select_" + faction;

      text_buttons.push_back(
        TextButton(
          id,
          full_names[name],
          32,
          InterfaceEvent::Data( InterfaceEvent::ID::FactionSelected, faction )
        )
          .Background( color )
      );
    }

    return text_buttons;
  }

  // inline std::vector<Element> CreateFactionSelectMenuUI()
  // {
  //   return {
  //     Panel( "faction_select_menu" )
  //       .Axis( Axis::Column )
  //       .Anchor( Anchor::Centered )
  //       .Children( CreateFactionButtons() ),
  //   };
  // }


};// namespace UI
