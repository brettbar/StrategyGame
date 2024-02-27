
#pragma once

#include "../../shared/global.hpp"

#include "../components/actor.hpp"

#include "faction_manager.hpp"

namespace fs = std::filesystem;

namespace Actor {
  class Manager {

public:
    static Manager *Get() {
      static Manager instance;
      return &instance;
    }

    std::map<str, Actor::Data> roster = {};

private:
    Manager() {
      init();
    }
    ~Manager() {}
    Manager( Manager const & ) = delete;

    void operator=( const Manager & ) = delete;


    void init() {
      build_actors_from_data();
      load_actor_assets();
      assign_faction_rosters();
    }

    void build_actors_from_data() {
      std::ifstream f( "src/data/actors.json" );
      {
        nlohmann::json js = nlohmann::json::parse( f );

        for ( auto &faction: js.items() ) {
          for ( auto &actor: faction.value().items() ) {

            str type = actor.value().at( "type" );
            str faction_id = faction.key();
            str actor_id = faction_id + "_" + type;
            str name = actor.value().at( "name" );
            str sprite_id = ( faction_id + "_" + name ).c_str();

            roster.emplace(
              actor_id,
              Actor::Data{
                Actor::type_lookup.at( type ),
                actor_id,
                name,
                faction_id,
                sprite_id,
              }
            );
          }
        }
      }
      f.close();
    }

    void load_actor_assets() {
      str root = "src/assets/images/actors";

      // for each faction
      for ( const auto &faction_folder: fs::directory_iterator( root ) ) {
        std::string faction = faction_folder.path().filename().generic_string();


        // for each file in the folder
        for ( const auto &actor_file:
              fs::directory_iterator( faction_folder ) ) {

          str path = actor_file.path().generic_string();
          str sprite =
            faction + "_" + actor_file.path().stem().generic_string();

          std::cout << "Adding to texture cache: " << sprite << "\n";

          LoadAsset(
            hstr{ sprite.c_str() },
            LoadImage( path.c_str() ),
            Global::texture_cache
          );

          LoadTexturePointFilter(
            hstr{ ( sprite + "_overview" ).c_str() },
            CropUnitImage( path.c_str() ),
            Global::texture_cache
          );
        }
      }
    }

    void assign_faction_rosters() {
      for ( auto &actor: roster ) {
        Faction::Component &faction =
          Faction::Manager::Get()->factions.at( actor.second.faction_id );

        faction.roster.actors.emplace( actor.second.type, actor.second );
      }
    }
  };
}// namespace Actor