// void DrawBorders(entt::registry &registry) {
//   // registry.sort<Tile>([](const Tile &lhs, const Tile &rhs) {
//   //   return rhs.position.y > lhs.position.y;
//   // });

//   auto view = registry.view<TileMap>();
//   auto mapEntity = view.front();
//   TileMap tileMap = view.get<TileMap>(mapEntity);

//   for (u32 x = 0; x < tileMap.width; x++) {
//     for (u32 y = 0; y < tileMap.height; y++) {
//       Tile &tile = tileMap.tiles[x][y];

//       // printf("%d\n", tile.owner);
//       if (tile.owner > -1 && tile.population > 100) {
//         printf("%d\n", tile.id);
//         f32 centerX = tile.position.x + 64;
//         f32 centerY = tile.position.y + 64;
//         Color color = BLACK;

//         switch (tile.owner) {
//         case 0:
//           color = RED;
//           break;
//         case 1:
//           color = SKYBLUE;
//           break;
//         case 2:
//           color = GREEN;
//           break;
//         case 3:
//           color = PURPLE;
//           break;
//         case 4:
//           color = ORANGE;
//           break;
//         }

//         Vector2 vertices[6] = {
//             {centerX, centerY + 64},      {centerX + 64, centerY + 32},
//             {centerX + 64, centerY - 32}, {centerX, centerY - 64},
//             {centerX - 64, centerY - 32}, {centerX - 64, centerY + 32},
//         };

//         Tile neighborNE = tileMap.tiles[x + 1][y - 1];
//         Tile neighborE = tileMap.tiles[x + 1][y];
//         Tile neighborSE = tileMap.tiles[x + 1][y + 1];
//         Tile neighborSW = tileMap.tiles[x][y + 1];
//         Tile neighborW = tileMap.tiles[x - 1][y];
//         Tile neighborNW = tileMap.tiles[x][y - 1];

//         // for (u32 i = 0; i < 5; i++) {
//         //   DrawLineEx(vertices[i], vertices[i + 1], 2, Fade(color,
//         0.5f));
//         // }

//         DrawLineEx(vertices[0], vertices[0 + 1], 2, Fade(color, 0.5f));
//         if (neighborNE.owner < 0) {
//           // draw NE border
//           DrawLineEx(vertices[0], vertices[0 + 1], 2, Fade(color, 0.5f));
//         }
//         if (neighborE.owner < 0) {
//           // draw E border
//           DrawLineEx(vertices[1], vertices[1 + 1], 2, Fade(color, 0.5f));
//         }
//         if (neighborSE.owner < 0) {
//           // draw SE border
//           DrawLineEx(vertices[2], vertices[2 + 1], 2, Fade(color, 0.5f));
//         }
//         if (neighborSW.owner < 0) {
//           // draw SW border
//           DrawLineEx(vertices[3], vertices[3 + 1], 2, Fade(color, 0.5f));
//         }
//         if (neighborW.owner < 0) {
//           // draw W border
//           DrawLineEx(vertices[4], vertices[4 + 1], 2, Fade(color, 0.5f));
//         }
//         if (neighborNW.owner < 0) {
//           // draw NW border
//           DrawLineEx(vertices[5], vertices[0], 2, Fade(color, 0.5f));
//         }
//       }
//     }
//   }
// }
// void DrawSingleBorder(Tile tile) {
//   f32 centerX = tile.position.x + 64;
//   f32 centerY = tile.position.y + 64;
//   Color color = BLACK;

//   Vector2 vertices[6] = {
//       {centerX, centerY + 64},      {centerX + 64, centerY + 32},
//       {centerX + 64, centerY - 32}, {centerX, centerY - 64},
//       {centerX - 64, centerY - 32}, {centerX - 64, centerY + 32},
//   };

//   switch (tile.owner) {
//   case 0:
//     color = RED;
//     break;
//   case 1:
//     color = SKYBLUE;
//     break;
//   case 2:
//     color = GREEN;
//     break;
//   case 3:
//     color = PURPLE;
//     break;
//   case 4:
//     color = ORANGE;
//     break;
//   }

//   for (u32 i = 0; i < 5; i++) {
//     DrawLineEx(vertices[i], vertices[i + 1], 2, Fade(color, 0.5f));
//   }
//   DrawLineEx(vertices[5], vertices[0], 2, Fade(color, 0.5f));
// }
