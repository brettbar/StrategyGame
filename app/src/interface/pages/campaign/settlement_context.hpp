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
      .Axis( Axis::Column )
      .Children( {
        TextButton( "train_hastati" )
          .Text( "Train", 24 )
          .Background( GREEN )
          .SetEvent( InterfaceEvent::ID::SettlementContextTrainHastati ),
        DataPanel( "settlement_garrison" ).Update( []( Element &self ) {
          std::vector<Regiment> regiments =
            SettlementSystem::SelectedSettlementGarrisonList();

          for ( auto regiment: regiments )
          {
            std::string panel_id = "regiment_" + std::to_string( regiment.id );

            self.CreateElementForDatapoints(
              Panel( panel_id )
                .Children( {
                  TextLabel( panel_id + "_unit_type" ).Text( "Hastati", 24 ),
                  TextLabel( panel_id + "_unit_count" )
                    .Text( std::to_string( regiment.number ), 24 ),
                } )
            );
          }
        } ),
      } );
  }

  inline Element BuildingTab()
  {
    return Panel( "settlement_context_construction" )
      .Axis( Axis::Column )
      .Children( {
        TextLabel( "buildings_label" ).Text( "Buildings", 26 ),
        TextButton( "build_farm" )
          .Text( "Build Farm", 26 )
          .SetEvent( InterfaceEvent::Data{
            InterfaceEvent::ID::SettlementContextConstructBuilding,
            "farm",
          } )
          .Background( BLUE ),
        Panel( "settlement_context_building_labels" )
          .Children( {
            TextLabel( "building_name" )
              .Text( "Building", 24 )
              .Background( PURPLE ),
            TextLabel( "producing" )
              .Text( "Producing", 24 )
              .Background( PURPLE )
              .Margins( { Margins{ 4, 4, 0, 0 } } ),
            TextLabel( "consuming" )
              .Text( "Consuming", 24 )
              .Background( PURPLE ),
          } ),
        DataPanel( "settlement_context_building_list" )
          .Axis( Axis::Column )
          .Update( []( Element &self ) {
            std::vector<Buildings::Building> buildings =
              SettlementSystem::SelectedSettlementBuildingList();

            for ( auto building: buildings )
            {

              std::string building_name =
                "building_list_item_" + building.name_str;

              self.CreateElementForDatapoints(
                Panel( "building_list_item_" + building.name_str )
                  .Children( {
                    TextLabel( building.name_str )
                      .Text( "Farm", 24 )
                      .Background( GREEN ),
                    TextLabel( building.name_str + "_count" )
                      .Text( "0", 24 )
                      .Background( BLACK ),
                    TextButton( "open_production_menu_" + building.name_str )
                      .Text( "+", 24 ),
                    Panel( "building_producing_list" ),
                    Panel( "building_using_list" ),
                  } )
              );
            }
          } ),
      } );
  }

  inline Element ResourceTab()
  {
    return Panel( "settlement_context_resources" )
      .Axis( Axis::Column )
      .Children( {
        TextLabel( "settlement_resource_list_label" )
          .Text( "Resource List", 26 ),
        DataPanel( "settlement_resource_list" )
          .Axis( Axis::Column )
          .Update( []( Element &self ) {
            Settlement::Component selected_settlement =
              SettlementSystem::ReadSelectedComponent();


            for ( auto [resource, count]: selected_settlement.raw_materials )
            {
              auto id =
                Resources::GetRawMaterialName( resource ) + "_data_point";

              self.CreateElementForDatapoints(

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
                      .Text( std::to_string( count ), 24 )
                      .Background( BLACK ),
                  } )
              );
            }
          } ),
      } );
  }

  inline Element CreateSettlementContextPanel()
  {
    return Panel( "settlement_context_panel" )
      .Anchor( Anchor::BottomMid )
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
          .Axis( Axis::Column )
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
                  .Text( "Uninhabited", 26 )
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
                  .Text( "Uninhabited", 26 )
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
                  .Text( "Uninhabited", 26 )
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
