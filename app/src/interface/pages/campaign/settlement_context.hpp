#pragma once
#include "../../element.hpp"
#include "../../ui_utils.hpp"

#include "../../../world/systems/settlement_system.hpp"

#include "../../builders.hpp"

namespace UI
{
  inline Element MilitaryTab()
  {
    return Panel( "settlement_context_military" )
      .SetAxis( Axis::Column )
      .Children( {
        TextButton( "train_hastati" )
          .SetText( "Train", 24 )
          .Background( GREEN )
          .SetEvent( InterfaceEvent::ID::SettlementContextTrainHastati ),
        DataPanel( "settlement_garrison" )
          .UpdateData( []( std::map<std::string, Element> &data_points ) {
            std::vector<Regiment> regiments =
              SettlementSystem::SelectedSettlementGarrisonList();

            for ( auto regiment: regiments )
            {
              std::string panel_id =
                "regiment_" + std::to_string( regiment.id );

              if ( !data_points.contains( panel_id ) )
              {

                Element panel =
                  Panel( panel_id )
                    .Children(
                      { TextLabel( panel_id + "_unit_type" )
                          .SetText( "Hastati", 24 ),
                        TextLabel( panel_id + "_unit_count" )
                          .SetText( std::to_string( regiment.number ), 24 ) }
                    );

                panel.Enable();

                data_points.insert_or_assign( panel_id, panel );
              }
            }
          } ),
      } );
  }

  inline Element BuildingTab()
  {
    return Panel( "settlement_context_construction" )
      .SetAxis( Axis::Column )
      .Children( {
        TextLabel( "buildings_label" ).SetText( "Buildings", 26 ),
        TextButton( "build_farm" )
          .SetText( "Build Farm", 26 )
          .SetEvent( InterfaceEvent::Data{
            InterfaceEvent::ID::SettlementContextConstructBuilding,
            "farm",
          } )
          .Background( BLUE ),
        Panel( "settlement_context_building_labels" )
          .Children( {
            TextLabel( "building_name" )
              .SetText( "Building", 24 )
              .Background( PURPLE ),
            TextLabel( "producing" )
              .SetText( "Producing", 24 )
              .Background( PURPLE )
              .Margins( { Margins{ 4, 4, 0, 0 } } ),
            TextLabel( "consuming" )
              .SetText( "Consuming", 24 )
              .Background( PURPLE ),
          } ),
        DataPanel( "settlement_context_building_list" )
          .SetAxis( Axis::Column )
          .UpdateData( []( std::map<std::string, Element> &data_points ) {
            std::vector<Buildings::Building> buildings =
              SettlementSystem::SelectedSettlementBuildingList();

            for ( auto building: buildings )
            {

              std::string building_name =
                "building_list_item_" + building.name_str;

              // TODO left off here
              if ( !data_points.contains( building_name ) )
              {
                Element panel =
                  Panel( "building_list_item_" + building.name_str )
                    .Children( {
                      TextLabel( building.name_str )
                        .SetText( "Farm", 24 )
                        .Background( GREEN ),
                      TextLabel( building.name_str + "_count" )
                        .SetText( "0", 24 )
                        .Background( BLACK ),
                      TextButton( "open_production_menu_" + building.name_str )
                        .SetText( "+", 24 ),
                      Panel( "building_producing_list" ),
                      Panel( "building_using_list" ),
                    } );

                panel.Enable();

                data_points.insert_or_assign( building_name, panel );
              }
              else
              {
                data_points.at( building_name )
                  .children[2]
                  .UpdateText( std::to_string( buildings.size() ) );
              }
            }
          } ),
      } );
  }

  inline Element ResourceTab()
  {
    return Panel( "settlement_context_resources" )
      .SetAxis( Axis::Column )
      .Children( {
        TextLabel( "settlement_resource_list_label" )
          .SetText( "Resource List", 26 ),
        DataPanel( "settlement_resource_list" )
          .SetAxis( Axis::Column )
          .UpdateData( []( std::map<std::string, Element> &existing_ids ) {
            Settlement::Component selected_settlement =
              SettlementSystem::ReadSelectedComponent();

            for ( auto [resource, count]: selected_settlement.raw_materials )
            {
              if ( !existing_ids.contains(
                     Resources::GetRawMaterialName( resource ) + "_data_point"
                   ) )
              {
                Element resource_panel =
                  Panel(
                    Resources::GetRawMaterialName( resource ) + "_data_point"
                  )
                    .Children( {
                      TextureLabel(
                        Resources::GetRawMaterialName( resource ) +
                        "_data_point_label"
                      )
                        .SetTexture( GetRawMaterialName( resource ) + ".png" ),
                      TextLabel(
                        Resources::GetRawMaterialName( resource ) +
                        "_data_point_value"
                      )
                        .SetText( std::to_string( count ), 24 )
                        .Background( BLACK ),
                    } );

                // Element resource_panel =
                //   TextLabel(
                //     Resources::GetRawMaterialName( resource ) +
                //     "_data_point"
                //   )
                //     .SetText( "test", 24 )
                //     .Background( YELLOW );

                existing_ids.insert_or_assign(
                  Resources::GetRawMaterialName( resource ) + "_data_point",
                  resource_panel
                );

                existing_ids
                  .at(
                    Resources::GetRawMaterialName( resource ) + "_data_point"
                  )
                  .Register();

                existing_ids
                  .at(
                    Resources::GetRawMaterialName( resource ) + "_data_point"
                  )
                  .Enable();
              }
            }
          } ),
      } );
  }

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
            TextureButton( "settlement_context_tab_garrison" )
              .SetEvent( InterfaceEvent::ID::SettlementContextMilitaryTab ),
            TextureButton( "settlement_context_tab_resources" )
              .SetEvent( InterfaceEvent::ID::SettlementContextResourcesTab ),
            TextureButton( "settlement_context_tab_construction" )
              .SetEvent( InterfaceEvent::ID::SettlementContextConstructionTab ),
            TextureButton( "settlement_context_tab_population" )
              .SetEvent( InterfaceEvent::ID::SettlementContextPopulationTab ),
            TextureButton( "settlement_context_tab_culture" ),
            TextureButton( "settlement_context_tab_religion" ),
          } ),
        StackPanel( "settlement_context_content" )
          .Background( RED )
          .On(
            InterfaceUpdate::ID::SettlementContextMilitaryTab,
            []( Element &self, InterfaceUpdate::Update update ) {
              self.SwitchChild( 0 );
            }
          )
          .On(
            InterfaceUpdate::ID::SettlementContextPopulationTab,
            []( Element &self, InterfaceUpdate::Update update ) {
              self.SwitchChild( 3 );
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
            MilitaryTab(),
            ResourceTab(),
            BuildingTab(),
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

          } ),
      } );
  }

}// namespace UI
