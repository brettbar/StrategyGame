#include "../../../builders.hpp"
#include "../../../element.hpp"

namespace UI
{
  inline sptr<Element> CreateSettlementContextTabConstruction()
  {
    return GridPanel( "settlement_context_construction", 1, 3 )
      .Children( {
        Slot( { 0, 0, 0, 0 }, TextLabel( "buildings_label", "Buildings", 26 ) ),
        Slot(
          { 0, 0, 1, 1 },
          TextButton( "build_farm", "Build Farm", 26 )
            .SetEvent( InterfaceEvent::Data{
              InterfaceEvent::ID::SettlementContextConstructBuilding,
              "farm",
            } )
            .Background( BLUE )
        ),
        Slot(
          { 0, 0, 2, 2 },
          GridPanel( "settlement_context_building_labels", 3, 1 )
            .Children( {
              Slot(
                { 0, 0, 0, 0 },
                TextLabel( "building_name", "Building", 24 )
                  .Background( PURPLE )
              ),
              Slot(
                { 1, 1, 0, 0 },
                TextLabel( "producing", "Producing", 24 )
                  .Background( PURPLE )
                  .Margins( { Margins{ 4, 4, 0, 0 } } )
              ),
              Slot(
                { 2, 2, 0, 0 },
                TextLabel( "consuming", "Consuming", 24 ).Background( PURPLE )
              ),
            } )
        ),
      } );
  }

  // inline Element BuildingTab()
  // {
  //   return Panel( "settlement_context_construction" )
  //     .Axis( Axis::Column )
  //     .Children( {
  //       TextLabel( "buildings_label" ).Text( "Buildings", 26 ),
  //       TextButton( "build_farm" )
  //         .Text( "Build Farm", 26 )
  //         .SetEvent( InterfaceEvent::Data{
  //           InterfaceEvent::ID::SettlementContextConstructBuilding,
  //           "farm",
  //         } )
  //         .Background( BLUE ),
  //       Panel( "settlement_context_building_labels" )
  //         .Children( {
  //           TextLabel( "building_name" )
  //             .Text( "Building", 24 )
  //             .Background( PURPLE ),
  //           TextLabel( "producing" )
  //             .Text( "Producing", 24 )
  //             .Background( PURPLE )
  //             .Margins( { Margins{ 4, 4, 0, 0 } } ),
  //           TextLabel( "consuming" )
  //             .Text( "Consuming", 24 )
  //             .Background( PURPLE ),
  //         } ),
  //       DataPanel( "settlement_context_building_list" )
  //         .Axis( Axis::Column )
  //         .Update( []( Element &self ) {
  //           std::vector<Buildings::Building> buildings =
  //             SettlementSystem::SelectedSettlementBuildingList();

  //           for ( auto building: buildings )
  //           {

  //             std::string building_name =
  //               "building_list_item_" + building.name_str;

  //             self.CreateElementForDatapoints(
  //               Panel( "building_list_item_" + building.name_str )
  //                 .Children( {
  //                   TextLabel( building.name_str )
  //                     .Text( "Farm", 24 )
  //                     .Background( GREEN ),
  //                   TextLabel( building.name_str + "_count" )
  //                     .Text( "0", 24 )
  //                     .Background( BLACK ),
  //                   TextButton( "open_production_menu_" + building.name_str )
  //                     .Text( "+", 24 ),
  //                   Panel( "building_producing_list" ),
  //                   Panel( "building_using_list" ),
  //                 } )
  //             );
  //           }
  //         } ),
  //     } );
  // }

};// namespace UI
