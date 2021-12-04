#include "map.hpp"
#include <raylib.h>

namespace Map {

void CreateTerrain(entt::registry &registry, u32 mapWidth, u32 mapHeight) {
  Vector2 position = {0.0f, 0.0f};

  for (u32 y = 0; y < mapHeight; y++) {
    for (u32 x = 0; x < mapWidth; x++) {
      entt::entity entity = registry.create();
      Tile tile = Tile{};
      tile.id = x + y * 128;
      tile.owner = -1;
      tile.population = 0;
      tile.name = "";

      if (y % 2 == 1)
        tile.position = {position.x + x * 128 + 64, position.y + y * 96};
      else
        tile.position = {position.x + x * 128, position.y + y * 96};

      tile.coord = {(f32)x, (f32)y};

      tile.neighborCoords = {
          {tile.coord.x + 1, tile.coord.y - 1},
          {tile.coord.x + 1, tile.coord.y},
          {tile.coord.x + 1, tile.coord.y + 1},
          {tile.coord.x, tile.coord.y + 1},
          {tile.coord.x - 1, tile.coord.y},
          {tile.coord.x, tile.coord.y - 1},
      };

      // tilemap.tiles[x][y] = tile;
      registry.emplace<Tile>(entity, tile.id, tile.coord, tile.position,
                             tile.owner, tile.population, tile.name);
    }
  }

  // registry.sort<Tile>(
  //     [](const Tile &lhs, const Tile &rhs) { return rhs.id > lhs.id; });
}

void UpdateProvinces(entt::registry &registry) {
  auto tiles = registry.view<Tile>();
  tiles.each([](Tile &tile) {
    if (tile.owner > -1 && tile.population >= 0)
      tile.population += 25;
  });
}

void SetProvinceOwner(entt::registry &registry, u32 owner, Vector2 clickPos) {
  i32 tileId = determineTileIdFromClick(clickPos);
  assert(tileId >= 0);

  auto tiles = registry.view<Tile>();

  tiles.each([tileId, owner](Tile &tile) {
    if (tile.id == (u32)tileId) {
      tile.owner = owner;
      switch (tile.owner) {
      case 0:
        tile.name = "Tomato Town";
        break;
      case 1:
        tile.name = "Athens";
        break;
      case 2:
        tile.name = "Lugudunon";
        break;
      case 3:
        tile.name = "Carthage";
        break;
      case 4:
        tile.name = "Persepolis";
        break;
      }
    }
  });
}

void DrawTerrain(entt::registry &registry, Texture2D hex, Rectangle frameRec) {
  auto tiles = registry.view<Tile>();

  tiles.each([hex, frameRec](Tile tile) {
    DrawTextureRec(hex, frameRec, tile.position, WHITE);
  });
}

void DrawProvinces(entt::registry &registry, bool debug, Texture2D village) {
  auto tiles = registry.view<Tile>();

  tiles.each([village, debug](Tile &tile) {
    // str idString = std::to_string(tile.id);
    // const char *idText = idString.c_str();
    // DrawText(idText, tile.position.x + 16.0, tile.position.y + 16.0, 14,
    // WHITE);

    // str ownerString = std::to_string(tile.owner);
    // const char *ownertext = ownerString.c_str();
    // DrawText(ownertext, tile.position.x + 48.0, tile.position.y + 16.0, 14,
    //          BLUE);

    // str coordString =
    //     std::to_string((u32)tile.coord.x) + "," +
    //     std::to_string((u32)tile.coord.y);
    // const char *coordText = coordString.c_str();
    // if (debug)
    //   DrawText(coordText, tile.position.x + 16.0, tile.position.y + 16.0, 14,
    //   BLUE);

    str popString = std::to_string(tile.population);
    const char *text = popString.c_str();
    if (debug)
      DrawText(text, tile.position.x + 16.0, tile.position.y + 32.0, 14, RED);

    if (tile.population >= 100) {
      // DrawSingleBorder(tile);
      DrawTextureV(village, tile.position, WHITE);
      if (tile.name != "")
        DrawText(tile.name.c_str(), tile.position.x + 50.0,
                 tile.position.y + 86.0, 14, WHITE);
    }
  });

  DrawBorders(registry);
}

void DrawBorders(entt::registry &registry) {
  // registry.sort<Tile>(
  //     [](const Tile &lhs, const Tile &rhs) { return rhs.id > lhs.id; });

  auto tiles = registry.view<Tile>();

  for (auto entity : tiles) {
    Tile &tile = tiles.get<Tile>(entity);

    if (tile.owner > -1 && tile.population > 100) {
      f32 centerX = tile.position.x + 64;
      f32 centerY = tile.position.y + 64;
      Color color = BLACK;

      switch (tile.owner) {
      case 0:
        color = RED;
        break;
      case 1:
        color = SKYBLUE;
        break;
      case 2:
        color = GREEN;
        break;
      case 3:
        color = PURPLE;
        break;
      case 4:
        color = ORANGE;
        break;
      }

      Vector2 vertices[6] = {
          {centerX, centerY + 64},      {centerX + 64, centerY + 32},
          {centerX + 64, centerY - 32}, {centerX, centerY - 64},
          {centerX - 64, centerY - 32}, {centerX - 64, centerY + 32},
      };

      printf("%d\n", tile.id);
      //   Tile neighborNE = tileMap.tiles[x + 1][y - 1];
      //   Tile neighborE = tileMap.tiles[x + 1][y];
      //   Tile neighborSE = tileMap.tiles[x + 1][y + 1];
      //   Tile neighborSW = tileMap.tiles[x][y + 1];
      //   Tile neighborW = tileMap.tiles[x - 1][y];
      //   Tile neighborNW = tileMap.tiles[x][y - 1];

      //   // for (u32 i = 0; i < 5; i++) {
      //   //   DrawLineEx(vertices[i], vertices[i + 1], 2, Fade(color, 0.5f));
      //   // }

      //   DrawLineEx(vertices[0], vertices[0 + 1], 2, Fade(color, 0.5f));

      // @TODO
        // if (tile.neighborCoords[0].x > -1 && tile.neighborCoords.y > -1) {
        //   // draw NE border
        //   DrawLineEx(vertices[0], vertices[0 + 1], 2, Fade(color, 0.5f));
        // }
        // if (neighborE.owner < 0) {
        //   // draw E border
        //   DrawLineEx(vertices[1], vertices[1 + 1], 2, Fade(color, 0.5f));
        // }
        // if (neighborSE.owner < 0) {
        //   // draw SE border
        //   DrawLineEx(vertices[2], vertices[2 + 1], 2, Fade(color, 0.5f));
        // }
        // if (neighborSW.owner < 0) {
        //   // draw SW border
        //   DrawLineEx(vertices[3], vertices[3 + 1], 2, Fade(color, 0.5f));
        // }
        // if (neighborW.owner < 0) {
        //   // draw W border
        //   DrawLineEx(vertices[4], vertices[4 + 1], 2, Fade(color, 0.5f));
        // }
        // if (neighborNW.owner < 0) {
        //   // draw NW border
        //   DrawLineEx(vertices[5], vertices[0], 2, Fade(color, 0.5f));
        // }
      // }
    }
  }

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
}

void DrawSingleBorder(Tile tile) {
  f32 centerX = tile.position.x + 64;
  f32 centerY = tile.position.y + 64;
  Color color = BLACK;

  Vector2 vertices[6] = {
      {centerX, centerY + 64},      {centerX + 64, centerY + 32},
      {centerX + 64, centerY - 32}, {centerX, centerY - 64},
      {centerX - 64, centerY - 32}, {centerX - 64, centerY + 32},
  };

  switch (tile.owner) {
  case 0:
    color = RED;
    break;
  case 1:
    color = SKYBLUE;
    break;
  case 2:
    color = GREEN;
    break;
  case 3:
    color = PURPLE;
    break;
  case 4:
    color = ORANGE;
    break;
  }

  for (u32 i = 0; i < 5; i++) {
    DrawLineEx(vertices[i], vertices[i + 1], 2, Fade(color, 0.5f));
  }
  DrawLineEx(vertices[5], vertices[0], 2, Fade(color, 0.5f));
}

i32 determineTileIdFromClick(Vector2 clickPos) {
  Vector2 *target = determineTilePos(clickPos);
  if (target == nullptr)
    return -1;

  i32 row = target->y / 96.0;
  i32 column;

  if (row % 2 == 1) {
    column = (target->x - 64.0) / 128.0;
  } else {
    column = target->x / 128.0;
  }

  return column + row * 128;
}

Vector2 *determineTilePos(Vector2 inputPos) {
  i32 x = inputPos.x;
  i32 y = inputPos.y;
  i32 gridHeight = 96;
  i32 gridWidth = 128;
  i32 halfWidth = gridWidth / 2;

  // Find the row and column of the box that the point falls in.
  int row = (int)(y / gridHeight);
  int column;

  bool rowIsOdd = row % 2 == 1;

  // Is the row an odd number?
  if (rowIsOdd) // Yes: Offset x to match the indent of the row
    column = (int)((x - halfWidth) / gridWidth);
  else // No: Calculate normally
    column = (int)(x / gridWidth);

  // Work out the position of the point relative to the box it is in
  f32 relY = y - (row * gridHeight);
  f32 relX;

  if (rowIsOdd)
    relX = (x - (column * gridWidth)) - halfWidth;
  else
    relX = x - (column * gridWidth);

  f32 c = 32.0;
  f32 m = c / halfWidth;

  // Work out if the point is above either of the hexagon's top edges
  if (relY < (-m * relX) + c) { // LEFT edge
    row--;
    if (!rowIsOdd)
      column--;
  } else if (relY < (m * relX) - c) { // RIGHT edge
    row--;
    if (rowIsOdd)
      column++;
  }

  f32 tileOrigX = (column * 128.0) + 64.0;
  f32 tileOrigY = (row * 96.0) + 64.0;

  if (row % 2 == 1) {
    tileOrigX += 64.0;
  }
  return new Vector2{tileOrigX, tileOrigY};
}
}; // namespace Map
