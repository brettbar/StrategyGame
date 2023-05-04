#pragma once

#include "../../shared/common.hpp"

#include "../element.hpp"
#include "../ui_utils.hpp"


namespace UI
{

  inline std::vector<UI::Element> CreateCampaignUI()
  {
    return {
      Panel( "settlement_context_panel" )
        .Background( Fade( BLACK, 0.5 ) )
        // .ListensFor( { InterfaceUpdate::ID::SettlementContext } )
        .Children( {
          Panel( "settlement_context_tab_group" )
            .SetAxis( Axis::Column )
            .Background( BLUE ),
          StackPanel( "settlement_context_content" )
            .Background( RED )
            .Children( {
              Panel( "settlement_context_overview" )
                .Children( {
                  TextLabel( "settlement_name" ).SetText( "Uninhabited", 26 ),
                  TextLabel( "settlement_population" )
                    .SetText( "Uninhabited", 26 ),
                  TextLabel( "settlement_development" )
                    .SetText( "Uninhabited", 26 ),
                } ),
            } ),

          Panel( "actor_context_panel" )
            .Background( Fade( BLACK, 0.5 ) )
            // .ListensFor( { InterfaceUpdate::ID::ActorContext } )
            .Children( {
              Panel( "actor_actions_panel" )
                .Children( {
                  TextButton( "actor_spawn_settlement_button" )
                    .SetText( "Spawn?", 26 )
                    .SetEvent( InterfaceEvent::Data(
                      InterfaceEvent::ID::ActorSpawnSettlment
                    ) ),
                } ),
            } ),

        } ),
    };


    // return {
    //   UI::Panel(
    //     "settlement_context_panel",
    //     Fade( BLACK, 0.5 ),
    //     Axis::Row,
    //     Align::Start,
    //     Align::Start,
    //     // true,
    //     { InterfaceUpdate::ID::SettlementContext },
    //     // []( Panel &self ) {
    //     //   vec2 updated_pos = {
    //     //     ( (f32) GetScreenWidth() / 2 ) - ( 500 * SCALE / 2.0f ),
    //     //     (f32) GetScreenHeight() - 200 * SCALE,
    //     //   };
    //     //   vec2 updated_size = {
    //     //     500 * SCALE,
    //     //     200 * SCALE,
    //     //   };

    //     //   self.transform.x = updated_pos.x;
    //     //   self.transform.y = updated_pos.y;
    //     //   self.transform.width = updated_size.x;
    //     //   self.transform.height = updated_size.y;
    //     // },
    //     {
    //       UI::Panel(
    //         "settlement_context_tab_group",
    //         BLUE,
    //         Axis::Column,
    //         Align::Start,
    //         Align::Start,
    //         {},
    //         {
    //           // TextureButton::Create(
    //           //   "settlement_context_tab_overview",
    //           //   true,

    //           //   []() {
    //           //     printf( "Settlement tab button pressed\n" );
    //           //     entt::entity content =
    //           //       lookup.at( "settlement_context_content" );
    //           //     StackPanel &sp = Get<StackPanel>( content );
    //           //     SwitchChild( sp, 0 );
    //           //   }
    //           // ),
    //           // TextureButton::Create(
    //           //   "settlement_context_tab_construction",
    //           //   true,
    //           //   []() {
    //           //     printf( "Settlement tab button pressed\n" );
    //           //     entt::entity content =
    //           //       lookup.at( "settlement_context_content" );
    //           //     StackPanel &sp = Get<StackPanel>( content );
    //           //     SwitchChild( sp, 1 );
    //           //   }

    //           // ),
    //           // TextureButton::Create( "settlement_context_tab_population", true ),
    //           // TextureButton::Create( "settlement_context_tab_resources", true ),
    //           // TextureButton::Create( "settlement_context_tab_culture", true ),
    //           // TextureButton::Create( "settlement_context_tab_religion", true ),
    //           // TextureButton::Create(
    //           //   "settlement_context_tab_construction", true
    //           // ),
    //           // TextureButton::Create( "settlement_context_tab_garrison", true ),
    //         },
    //       ),

    //       Create<StackPanel>( {
    //         "settlement_context_content",
    //         RED,
    //         {
    //           Create<Panel>( {
    //             "settlement_context_overview",
    //             BLACK,
    //             Axis::Row,
    //             Align::Start,
    //             Align::Start,
    //             {
    //               Create<TextLabel>( {
    //                 "settlement_name",
    //                 "Uninhabited",
    //                 32,
    //                 BLACK,
    //                 WHITE,
    //               } ),
    //               Create<TextLabel>( {
    //                 "settlement_population",
    //                 "0",
    //                 32,
    //                 BLACK,
    //                 WHITE,
    //               } ),
    //               Create<TextLabel>( {
    //                 "settlement_development",
    //                 "Uninhabited",
    //                 32,
    //                 BLACK,
    //                 WHITE,
    //               } ),
    //             },
    //           } ),
    //           Create<Panel>(

    //             {
    //               "settlement_context_construction",
    //               BLACK,
    //               Axis::Row,
    //               Align::Start,
    //               Align::Start,
    //               {
    //                 Create<TextLabel>(
    //                   { "building_0", "building", 32, BLACK, WHITE }
    //                 ),
    //               },
    //             }
    //           ),
    //         },
    //       } ),
    //     },
    //   ),
    //   Create<Panel>( {
    //     "actor_context_panel",
    //     Fade( BLACK, 0.5 ),
    //     Axis::Row,
    //     Align::Start,
    //     Align::Start,
    //     true,
    //     { InterfaceUpdate::ID::ActorContext },
    //     []( Panel &self ) {
    //       vec2 updated_pos = {
    //         ( (f32) GetScreenWidth() / 2 ) - ( 500 * SCALE / 2.0f ),
    //         (f32) GetScreenHeight() - 200 * SCALE,
    //       };

    //       vec2 updated_size = {
    //         500 * SCALE,
    //         200 * SCALE,
    //       };

    //       self.transform.x = updated_pos.x;
    //       self.transform.y = updated_pos.y;
    //       self.transform.width = updated_size.x;
    //       self.transform.height = updated_size.y;
    //     },
    //     {
    //       Create<Panel>( {
    //         "actor_actions_panel",
    //         BLACK,
    //         Axis::Row,
    //         Align::Start,
    //         Align::Start,
    //         {
    //           Create<TextButton>( {
    //             "actor_spawn_settlement_button",
    //             "Spawn?",
    //             26,
    //             PURPLE,
    //             WHITE,
    //             InterfaceEvent::Data( InterfaceEvent::ID::ActorSpawnSettlment ),
    //           } ),
    //         },
    //       } ),
    //     },
    //   } ),
    // };
  }

};// namespace UI
