#pragma once

#include "../../../../world/systems/settlement_system.hpp"
#include "../../../builders.hpp"
#include "../../../element.hpp"
#include "../../../ui_utils.hpp"


namespace UI
{
  inline sptr<Element> RecruitmentWindow()
  {
    return GridPanel( "recruitment_window", 3, 3 )
      .Children( {
        Slot(
          { 0, 0, 0, 0 },
          TextButton( "train_hastati", "Train", 24 )
            .Background( GREEN )
            .SetEvent( InterfaceEvent::ID::SettlementContextTrainHastati )
        ),
      } );
  }

  inline sptr<Element> TrainingWindow()
  {
    return GridPanel( "training_window", 3, 3 )
      .Children( {
        Slot( { 0, 0, 0, 0 }, TextLabel( "training", "Training", 24 ) ),
      } );
  }

  // inline Element GarrisonWindow()
  // {
  //   return DataPanel( "settlement_garrison" )
  //     // .MaximumSize()
  //     .Update( []( Element &self ) {
  //       std::vector<Regiment> regiments =
  //         SettlementSystem::SelectedSettlementGarrisonList();

  //       for ( auto regiment: regiments )
  //       {
  //         std::string panel_id = "regiment_" + std::to_string( regiment.id );

  //         self.CreateElementForDatapoints(
  //           Panel( panel_id )
  //             .Axis( Axis::Column )
  //             .Children( {
  //               TextureLabel( panel_id + "_unit_type" )
  //                 .Texture( "romans_hastati_texture_overview" ),
  //               TextLabel( panel_id + "_unit_count" )
  //                 .Text( std::to_string( regiment.number ), 24 ),
  //             } )
  //         );
  //       }
  //     } );
  // }


  inline sptr<Element> CreateSettlementContextTabMilitary()
  {
    return GridPanel( "settlement_context_military", 3, 1 )
      .Children( {
        Slot( { 0, 0, 0, 0 }, RecruitmentWindow() ),
        Slot( { 1, 1, 0, 0 }, TrainingWindow() ),
        // GarrisonWindow(),
      } );
  }

}// namespace UI