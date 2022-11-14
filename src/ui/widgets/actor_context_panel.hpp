// #pragma once


// #include "../../renderer/textures.hpp"
// #include "../ui_components.hpp"

// namespace UI {

// inline entt::entity ActorActionsPanel();
// inline entt::entity ActorSpawnSettlementButton();

// inline entt::entity ActorContextPanel( std::vector<entt::entity> children ) {
//   Element elem = {
//     .id = "actor_context_panel",
//     .type = Type::BasePanel,
//     .enabled = false,
//     .transform =
//       {
//         .width = 540,
//         .height = 203,
//       },
//   };

//   BasePanel panel = {
//     {
//       .background = Fade( BLACK, 0.5 ),
//       .children_axis = Axis::ROW,
//       .children_horiz_align = Align::START,
//       .children_vert_align = Align::START,
//       .children = children,
//       // {
//       //   ActorActionsPanel(),
//       // },
//     },
//     .original_size =
//       {
//         .x = 540,
//         .y = 203,
//       },
//     // TODO cludgy, clean up
//     .update =
//       []( Element &elem, BasePanel &panel ) {
//         elem.transform = {
//           ( (f32) GetScreenWidth() / 2 ) -
//             ( panel.original_size.x * SCALE / 2.0f ),
//           (f32) GetScreenHeight() - panel.original_size.y * SCALE,
//           panel.original_size.x * SCALE,
//           panel.original_size.y * SCALE,
//         };
//       },
//   };

//   return CreateElement( panel, elem );
// }


// inline entt::entity ActorActionsPanel( std::vector<entt::entity> children ) {
//   Element elem = {
//     .id = "actor_actions_panel",
//     .type = Type::Panel,
//     .enabled = false,
//   };

//   Panel panel = {
//     .children = children,
//   };

//   return CreateElement( panel, elem );
// }


// inline entt::entity ActorSpawnSettlementButton() {
//   Element elem = {
//     .id = "actor_spawn_settlement_button",
//     .type = Type::TextButton,
//     .enabled = false,
//   };

//   TextButton button = {
//     {
//       .text = "Spawn Settlement",
//       .font_size = 18,
//       .text_color = WHITE,
//       .background = RED,
//     },
//     .clickable = false,
//   };


//   return CreateElement( button, elem );
// }

// };// namespace UI
