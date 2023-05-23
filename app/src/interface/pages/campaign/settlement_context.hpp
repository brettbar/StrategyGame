#pragma once
#include "../../element.hpp"
#include "../../ui_utils.hpp"

#include "../../../world/systems/settlement_system.hpp"

namespace UI
{
  inline Element CreateSettlementContextPanel()
  {
    return Panel( "settlement_context_panel" )
      .SetAnchor( Anchor::BottomMid )
      .FixedSize( 800, 350 )
      .Background( Fade( BLACK, 0.5 ) )
      .On(
        InterfaceUpdate::ID::SettlementContext,
        []( Element &self, InterfaceUpdate::Update update ) {
          if ( update.condition )
            self.Enable();
          else
            self.Disable();
        }
      )
      .Children( {
        Panel( "settlement_context_tab_group" )
          .SetAxis( Axis::Column )
          .Background( BLUE )
          .Children( {
            TextureButton( "settlement_context_tab_population" )
              .SetEvent( InterfaceEvent::ID::SettlementContextPopulationTab ),
            TextureButton( "settlement_context_tab_resources" )
              .SetEvent( InterfaceEvent::ID::SettlementContextResourcesTab ),
            TextureButton( "settlement_context_tab_construction" )
              .SetEvent( InterfaceEvent::ID::SettlementContextConstructionTab ),
            TextureButton( "settlement_context_tab_culture" ),
            TextureButton( "settlement_context_tab_religion" ),
            TextureButton( "settlement_context_tab_garrison" ),
          } ),
        StackPanel( "settlement_context_content" )
          .Background( RED )
          .On(
            InterfaceUpdate::ID::SettlementContextPopulationTab,
            []( Element &self, InterfaceUpdate::Update update ) {
              self.SwitchChild( 0 );
            }
          )
          .On(
            InterfaceUpdate::ID::SettlementContextResourcesTab,
            []( Element &self, InterfaceUpdate::Update update ) {
              self.SwitchChild( 1 );
            }
          )
          .On(
            InterfaceUpdate::ID::SettlementContextConstructionTab,
            []( Element &self, InterfaceUpdate::Update update ) {
              self.SwitchChild( 2 );
            }
          )
          .Children( {
            Panel( "settlement_context_overview" )
              .Children( {
                TextLabel( "settlement_name" )
                  .SetText( "Uninhabited", 26 )
                  .On(
                    InterfaceUpdate::ID::SettlementContext,
                    []( Element &self, InterfaceUpdate::Update update ) {
                      if ( update.condition )
                      {
                        self.UpdateText( update.json["name"] );
                      }
                    }
                  ),
                TextLabel( "settlement_population" )
                  .SetText( "Uninhabited", 26 )
                  .On(
                    InterfaceUpdate::ID::SettlementContext,
                    []( Element &self, InterfaceUpdate::Update update ) {
                      if ( update.condition )
                      {
                        self.UpdateText( update.json["population"] );
                      }
                    }
                  ),
                TextLabel( "settlement_development" )
                  .SetText( "Uninhabited", 26 )
                  .On(
                    InterfaceUpdate::ID::SettlementContext,
                    []( Element &self, InterfaceUpdate::Update update ) {
                      if ( update.condition )
                      {
                        self.UpdateText( update.json["development"] );
                      }
                    }
                  ),
              } ),
            Panel( "settlement_context_resources" )
              .Children( {
                TextLabel( "settlement_resource_list" )
                  .SetText( "Resource List", 26 ),
              } ),
            Panel( "settlement_context_construction" )
              .SetAxis( Axis::Column )
              .Children( {
                TextLabel( "buildings_label" ).SetText( "Buildings", 26 ),
                TextButton( "build_farm" )
                  .SetText( "Build Farm", 26 )
                  .SetEvent( InterfaceEvent::Data{
                    InterfaceEvent::ID::SettlementContextConstructBuilding,
                    "Farm",
                  } )
                  .Background( BLUE ),
                Panel( "settlement_context_building_labels" )
                  .Children( {
                    TextLabel( "building_name" )
                      .SetText( "Building", 24 )
                      .Background( PURPLE ),
                    TextLabel( "inputs" )
                      .SetText( "Inputs", 24 )
                      .Background( PURPLE )
                      .Margins( { Margins{ 4, 4, 0, 0 } } ),
                    TextLabel( "outputs" )
                      .SetText( "Outputs", 24 )
                      .Background( PURPLE ),
                  } ),
                Panel( "settlement_context_building_list" )
                  .SetAxis( Axis::Column )
                  .Children( {} )
                  .UpdateChildren( [](
                                     std::map<std::string, bool> &existing_ids,
                                     std::vector<Element> &children
                                   ) {
                    std::vector<std::string> buildings =
                      SettlementSystem::SelectedSettlementBuildingList();

                    for ( std::string building: buildings )
                    {
                      // TODO left off here
                      if ( !existing_ids.contains( building ) )
                      {
                        Element label = TextLabel( "farm" )
                                          .SetText( "Farm", 24 )
                                          .Background( GREEN );
                        label.Enable();

                        children.push_back( label );
                      }
                    }
                  } ),
                // .On(
                //   InterfaceUpdate::ID::SettlementContextConstructBuilding,
                //   []( Element &self, InterfaceUpdate::Update update ) {
                //     // TODO(rf) this aint gonna work. For updating children we should be doing something where
                //     // we check the ecs idk
                //     // But this aint gonna work
                //     self.children.push_back(
                //       TextLabel( "farm" )
                //         .SetText( update.update_txt, 24 )
                //         .Background( GREEN )
                //     );

                //     self.children[self.children.size() - 1].Enable();
                //   }
                // ),
              } ),
          } ),
      } );
  }

}// namespace UI
