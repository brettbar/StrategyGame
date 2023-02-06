#pragma once

#include "../../shared/common.hpp"

#include "../components/panel.hpp"
#include "../components/text_elements.hpp"

#include "../ui_system.hpp"

namespace UI {

  inline std::vector<entt::entity> CreateFactionButtons() {
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

    std::vector<entt::entity> text_buttons = {
      Create<TextLabel>( {
        "faction_select_label",
        "Select your faction",
        32,
        BLACK,
        WHITE,
        false,
        {},
      } ),
    };

    for ( const auto &[name, color]: factions ) {
      std::string faction = name;
      std::string id = "faction_select_" + faction;


      std::function<void()> action = [faction]() {
        Events::event_emitter.publish( Events::ButtonClick{
          "faction_select",
          faction,
        } );
      };

      text_buttons.push_back( Create<TextButton>(
        { id, full_names[name], 32, color, WHITE, false, {}, {} }
      ) );
    }

    return text_buttons;
  }

  inline std::vector<entt::entity> CreateFactionSelectMenuUI() {
    return {
      Create<Panel>( {
        "facton_select_menu",
        BLACK,
        Axis::Column,
        Align::Start,
        Align::Start,
        true,
        []( Panel &self ) {
          vec2 update_pos = {
            ( (f32) GetScreenWidth() / 2 ) - ( 200 * SCALE / 2.0f ),
            ( (f32) GetScreenHeight() / 2 ) - 200 * SCALE,
          };
          self.elem.transform.x = update_pos.x;
          self.elem.transform.y = update_pos.y;
        },
        CreateFactionButtons(),
      } ),
    };
  }


};// namespace UI
