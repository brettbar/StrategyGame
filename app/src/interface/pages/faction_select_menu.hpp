#pragma once

#include "../../shared/common.hpp"

#include "../ui_system.hpp"

namespace UI
{
  inline list<GridPanelElement::Slot> CreateFactionButtons()
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

    list<GridPanelElement::Slot> text_buttons = {
      GridPanelElement::Slot{
        { 0, 0, 0, 0 },
        TextLabel( "faction_select_label", "Select your faction", 32 ) },
    };

    u32 slot = 1;
    for ( const auto &[name, color]: factions )
    {
      std::string faction = name;
      std::string id = "faction_select_" + faction;

      text_buttons.push_back( GridPanelElement::Slot{
        { 0, 0, slot, slot },
        TextButton( id, full_names[name], 32 )
          .SetEvent(
            InterfaceEvent::Data( InterfaceEvent::ID::FactionSelected, faction )
          )
          .Background( color ),
      } );

      slot++;
    }

    return text_buttons;
  }

  inline sptr<Element> CreateFactionSelectMenuUI()
  {
    return GridPanel( "faction_select_root", 3, 3 )
      .FixedSize( GetScreenWidth(), GetScreenHeight() )
      .Children( {
        GridPanelElement::Slot{
          { 1, 1, 1, 1 },
          GridPanel( "faction_select_menu", 1, 8 )
            .Children( CreateFactionButtons() ),
        },
      } );
  }
};// namespace UI
