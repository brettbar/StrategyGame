// #include "../../../../../data/buildings.hpp"
// #include "../../../../world/systems/settlement.hpp"

// namespace UI {
//   inline sptr<Element> CreateSettlementContextTabConstruction() {
//     return GridPanel( "settlement_context_construction", 1, 4 )
//       .Children( {
//         GridSlot(
//           { 0, 0, 0, 0 }, TextLabel( "buildings_label", "Buildings", 26 )
//         ),
//         GridSlot(
//           { 0, 0, 1, 1 },
//           TextButton( "build_farm", "Build Farm", 26 )
//             .SetEvent( InterfaceEvent::Data{
//               InterfaceEvent::ID::SettlementContextConstructBuilding,
//               "farm",
//             } )
//             .Background( BLUE )
//         ),
//         GridSlot(
//           { 0, 0, 2, 2 },
//           GridPanel( "settlement_context_building_labels", 3, 1 )
//             .Children( {
//               GridSlot(
//                 { 0, 0, 0, 0 },
//                 TextLabel( "building_name", "Building", 24 )
//                   .Background( PURPLE )
//               ),
//               GridSlot(
//                 { 1, 1, 0, 0 },
//                 TextLabel( "producing", "Producing", 24 )
//                   .Background( PURPLE )
//                   .Margins( { Margins{ 4, 4, 0, 0 } } )
//               ),
//               GridSlot(
//                 { 2, 2, 0, 0 },
//                 TextLabel( "consuming", "Consuming", 24 ).Background( PURPLE )
//               ),
//             } )
//         ),
//         GridSlot(
//           { 0, 0, 3, 3 },
//           GridPanel( "settlement_context_building_list", 5, 4 )
//             .UpdateChildren( []( Element &self ) {
//               std::vector<Buildings::Building> buildings =
//                 Settlement::SelectedSettlementBuildingList();

//               for ( auto building: buildings ) {
//                 str building_name = "building_list_item_" + building.name_str;
//                 str id = "building_list_item_" + building.name_str;

//                 if ( !self.grid_panel->AlreadyHasChild( id ) ) {
//                   self.grid_panel->FillNextGridSlot(
//                     GridPanel( id, 1, 5 )
//                       .Children( {
//                         GridSlot(
//                           { 0, 0, 0, 0 },
//                           TextLabel( building.name_str, "Farm", 24 )
//                             .Background( GREEN )
//                         ),
//                         GridSlot(
//                           { 0, 0, 1, 1 },
//                           TextLabel( building.name_str + "_count", "0", 24 )
//                             .Background( BLACK )
//                         ),
//                         GridSlot(
//                           { 0, 0, 2, 2 },
//                           TextButton(
//                             "open_production_menu_" + building.name_str, "+", 24
//                           )
//                         )
//                         // GridPanel( "building_producing_list" ),
//                         // GridPanel( "building_using_list" ),
//                       } )
//                   );
//                 }
//               }
//             } )
//         ),
//       } );
//   }

// };// namespace UI
