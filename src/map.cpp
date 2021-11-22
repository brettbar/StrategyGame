#include "map.h"
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

      registry.emplace<Tile>(entity, tile.id, tile.position);
    }
  }
}

void DrawTerrain(entt::registry &registry, Texture2D hex, Rectangle frameRec) {
  auto tiles = registry.view<Tile>();

  tiles.each([hex, frameRec](Tile tile) {
    DrawTextureRec(hex, frameRec, tile.position, WHITE);
  });
}

void UpdateProvinces(entt::registry &registry) {
  auto tiles = registry.view<Tile>();
  // Vector2 *target = determineTilePos(clickPos);
  // assert(target != nullptr);

  tiles.each([] (Tile &tile) {
    if (tile.population > 0)
      tile.population += 1;
  });
  // int row = target->y / 96.0;
  // int column;

  // if (row % 2 == 1) {
  //   column = (target->x - 64.0) / 128.0;
  // } else {
  //   column = target->x / 128.0;
  // }

  // int targetId = column + row * 128;

  // printf("%d\n", targetId);

  // tiles.each([targetId](Tile &tile) {
  //   if (targetId == (i32)tile.id) {
  //     tile.population += 25;
  //   }
  // });
}

void DrawProvinces(entt::registry &registry, Texture2D village) {
  auto tiles = registry.view<Tile>();

  tiles.each([village](Tile &tile) {
    std::string popText = std::to_string(tile.population);
    const char *text = popText.c_str();
    DrawText(text, tile.position.x + 16.0, tile.position.y + 16.0, 14, WHITE);

    if (tile.population >= 100) {
      DrawBorder(tile);
      DrawTextureV(village, tile.position, WHITE);
      if (tile.name != "")
        DrawText(tile.name.c_str(), tile.position.x + 50.0,
               tile.position.y + 86.0, 14, WHITE);
    }
  });
}

void DrawBorder(Tile tile) {
  f32 centerX = tile.position.x + 64;
  f32 centerY = tile.position.y + 64;

  Vector2 vertices[6] = {
      {centerX, centerY + 64},      {centerX + 64, centerY + 32},
      {centerX + 64, centerY - 32}, {centerX, centerY - 64},
      {centerX - 64, centerY - 32}, {centerX - 64, centerY + 32},
  };

  for (u32 i = 0; i < 5; i++) {
    DrawLineEx(vertices[i], vertices[i + 1], 2, Fade(RED, 0.5f));
  }
  DrawLineEx(vertices[5], vertices[0], 2, Fade(RED, 0.5f));
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
