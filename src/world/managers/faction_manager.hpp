#pragma once

#include "../../shared/common.hpp"

#include "../components/faction_component.hpp"

namespace Faction {
  inline Color color_from_str( str color_s ) {
    map<str, Color> m = {
      { "red", RED },
      { "blue", BLUE },
      { "green", GREEN },
      { "purple", PURPLE },
      { "orange", ORANGE },
      { "pink", PINK },
      { "gray", GRAY },
      { "black", BLACK },
      { "white", WHITE },
      { "cyan", BLUE },
      { "gold", GOLD },
      { "yellow", YELLOW },
    };
    return m[color_s];
  }

  class Manager {
public:
    map<str, Faction::Component> factions = {};
    list<str> ids = {};
    map<str, Color> primary_colors = {};
    u32 num_factions = 0;

    static Manager *Get() {
      static Manager instance;
      return &instance;
    }

private:
    Manager() {
      init();
    }
    ~Manager() {}
    Manager( Manager const & ) = delete;
    void operator=( const Manager & ) = delete;

    inline void init() {
      factions = {};

      std::filesystem::path cwd = std::filesystem::current_path();

      std::cout << "Current Working Directory: " << cwd.generic_string()
                << std::endl;

      std::ifstream f( "data/factions.json" );
      {
        nlohmann::json js = nlohmann::json::parse( f );

        std::cout << js << std::endl;
        // std::cout << js["romans"] << std::endl;
        // std::cout << js["romans"]["adjectival"] << std::endl;
        for ( auto &element: js.items() ) {
          std::cout << element.key() << std::endl;
          std::cout << element.value() << std::endl;

          auto colors = element.value().at( "colors" );
          auto culture = element.value().at( "culture" );
          auto government = element.value().at( "government" );
          auto mobility = element.value().at( "mobility" );

          Faction::Component faction =
            { .id = element.key(),
              .colors =
                {
                  .primary = colors.at( "primary" ),
                  .secondary = colors.at( "secondary" ),
                },
              .culture =
                {
                  .adjective = culture.at( "adjective" ),
                  .denonym = culture.at( "denonym" ),
                  .settlement_names = culture.at( "settlement_names" ),
                },
              .government =
                {
                  .start_gov =
                    Faction::gov_from_str( government.at( "start_gov" ) ),
                  .empire_name = government.at( "empire_name" ),
                  .empire_leader_title = government.at( "empire_leader_title" ),
                  .republic_name = government.at( "republic_name" ),
                  .republic_leader_title =
                    government.at( "republic_leader_title" ),
                  .federation_name = government.at( "federation_name" ),
                  .federation_leader_title =
                    government.at( "federation_leader_title" ),
                },
              .mobility = {
                .type = Faction::from_str( mobility.at( "type" ) ),
              } };

          factions.emplace( faction.id, faction );
          ids.push_back( faction.id );
          primary_colors.emplace(
            faction.id, color_from_str( faction.colors.primary )
          );
          num_factions++;
        }
      }
      f.close();
    }
  };
};// namespace Faction