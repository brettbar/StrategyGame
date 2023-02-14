#pragma once

#include "../../shared/common.hpp"


#include "../components/element.hpp"
#include "../components/panel.hpp"
#include "../components/text_elements.hpp"
#include "../components/texture_elements.hpp"

#include "../ui_utils.hpp"


namespace UI {

  inline std::vector<ptr<Panel>> CreateCampaignUI() {
    return {
      Create<Panel>( {
        "settlement_context_panel",
        Fade( BLACK, 0.5 ),
        Axis::Row,
        Align::Start,
        Align::Start,
        true,
        []( Panel &self ) {
          vec2 updated_pos = {
            ( (f32) GetScreenWidth() / 2 ) - ( 500 * SCALE / 2.0f ),
            (f32) GetScreenHeight() - 200 * SCALE,
          };
          vec2 updated_size = {
            500 * SCALE,
            200 * SCALE,
          };

          self.transform.x = updated_pos.x;
          self.transform.y = updated_pos.y;
          self.transform.width = updated_size.x;
          self.transform.height = updated_size.y;
        },
        {
          Create<Panel>( {
            "settlement_context_tab_group",
            BLUE,
            Axis::Column,
            Align::Start,
            Align::Start,
            {
              // TextureButton::Create(
              //   "settlement_context_tab_overview",
              //   true,

              //   []() {
              //     printf( "Settlement tab button pressed\n" );
              //     entt::entity content =
              //       lookup.at( "settlement_context_content" );
              //     StackPanel &sp = Get<StackPanel>( content );
              //     SwitchChild( sp, 0 );
              //   }
              // ),
              // TextureButton::Create(
              //   "settlement_context_tab_construction",
              //   true,
              //   []() {
              //     printf( "Settlement tab button pressed\n" );
              //     entt::entity content =
              {},
              //       lookup.at( "settlement_context_content" );
              //     StackPanel &sp = Get<StackPanel>( content );
              //     SwitchChild( sp, 1 );
              //   }

              // ),
              // TextureButton::Create( "settlement_context_tab_population", true ),
              // TextureButton::Create( "settlement_context_tab_resources", true ),
              // TextureButton::Create( "settlement_context_tab_culture", true ),
              // TextureButton::Create( "settlement_context_tab_religion", true ),
              // TextureButton::Create(
              //   "settlement_context_tab_construction", true
              // ),
              // TextureButton::Create( "settlement_context_tab_garrison", true ),
            },
          } ),

          Create<StackPanel>( {

            "settlement_context_content",
            RED,
            {
              Create<Panel>( {
                "settlement_context_overview",
                BLACK,
                Axis::Row,
                Align::Start,
                Align::Start,
                {
                  Create<TextLabel>( {
                    "settlement_name",
                    "Uninhabited",
                    32,
                    BLACK,
                    WHITE,
                  } ),
                  Create<TextLabel>( {
                    "settlement_population",
                    "0",
                    32,
                    BLACK,
                    WHITE,
                  } ),
                  Create<TextLabel>( {
                    "settlement_development",
                    "Uninhabited",
                    32,
                    BLACK,
                    WHITE,
                  } ),
                },
              } ),
              Create<Panel>(

                {
                  "settlement_context_construction",
                  BLACK,
                  Axis::Row,
                  Align::Start,
                  Align::Start,
                  {
                    Create<TextLabel>(
                      { "building_0", "building", 32, BLACK, WHITE }
                    ),
                  },
                }
              ),
            },
          } ),
        },
      } ),
      Create<Panel>( {
        "actor_context_panel",
        Fade( BLACK, 0.5 ),
        Axis::Row,
        Align::Start,
        Align::Start,
        true,
        []( Panel &self ) {
          vec2 updated_pos = {
            ( (f32) GetScreenWidth() / 2 ) - ( 500 * SCALE / 2.0f ),
            (f32) GetScreenHeight() - 200 * SCALE,
          };

          vec2 updated_size = {
            500 * SCALE,
            200 * SCALE,
          };

          self.transform.x = updated_pos.x;
          self.transform.y = updated_pos.y;
          self.transform.width = updated_size.x;
          self.transform.height = updated_size.y;
        },
        {
          Create<Panel>( {
            "actor_actions_panel",
            BLACK,
            Axis::Row,
            Align::Start,
            Align::Start,
            {
              Create<TextButton>( {
                "actor_spawn_settlement_button", "Spawn?", 26, PURPLE, WHITE,
                // []() {
                //   printf( "Spawn Settlement clicked!!\n" );
                //   SettlementSystem::SpawnSettlement();

                //   // TODO handle this in main
                //   // Events::event_emitter.publish( Events::UIEvent{
                //   //   "actor_spawn_settlement_button",
                //   // } );
                // },
              } ),
            },
          } ),
        },
      } ),
    };
  }

};// namespace UI
