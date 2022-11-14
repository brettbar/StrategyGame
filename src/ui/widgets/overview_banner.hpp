// #pragma once

// #include "../ui_components.hpp"

// namespace UI {
// inline entt::entity OverviewBanner();
// inline entt::entity OverviewTabButton();


// inline entt::entity OverviewBanner() {
//   Element elem = {
//     .id = "overview_banner",
//     .type = Type::BasePanel,
//     .enabled = true,
//     .transform =
//       {
//         .x = 0,
//         .y = 0,
//         .width = 80,
//         .height = (f32) GetScreenHeight(),
//       },
//   };

//   BasePanel panel = {
//     {
//       .background = GRAY,
//       .children_axis = Axis::COLUMN,
//       .children_horiz_align = Align::START,
//       .children_vert_align = Align::START,
//       .children =
//         {

//         },
//     },
//     .update =
//       []( Element &elem, BasePanel &panel ) {
//         elem.transform = {
//           0,
//           0,
//           80,
//           (f32) GetScreenHeight(),
//         };
//       },
//   };

//   return CreateElement( panel, elem );
// }

// inline entt::entity OverviewTabButton( Texture2D texture ) {
//   Element elem = {
//     .id = "overview_tab_button",
//     .type = Type::TextButton,
//     .enabled = true,
//   };
// }


// };// namespace UI
