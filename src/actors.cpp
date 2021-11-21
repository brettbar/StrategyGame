#include "actors.h"

namespace Actors {

void Draw(entt::registry &registry) {
  entt::basic_view villagers = registry.view<Actors::RomanVillager>();
  villagers.each([](Actors::RomanVillager romanVillager) {
    DrawTextureV(
        romanVillager.sprite,
        {romanVillager.position.x - 64.0f, romanVillager.position.y - 64.0f},
        WHITE);
  });
}

void CreateNew(entt::registry &registry, Vector2 clickPos,
               Texture2D romanVillagerTexture) {
  entt::entity entity = registry.create();

  Actors::RomanVillager roman = Actors::RomanVillager{
      .position = clickPos,
      .destination = clickPos,
      .speed = 1.0f,
      .sprite = romanVillagerTexture,
  };

  registry.emplace<Actors::RomanVillager>(
      entity, roman.position, roman.destination, roman.speed, roman.sprite);
}

void SetDestinations(entt::registry &registry, Camera2D camera) {
  entt::basic_view villagers = registry.view<Actors::RomanVillager>();

  villagers.each([camera](Actors::RomanVillager &romanVillager) {
    Vector2 *tileOrig =
        determineTilePos(GetScreenToWorld2D(GetMousePosition(), camera));
    if (tileOrig != nullptr)
      romanVillager.destination = *tileOrig;
  });
}

void UpdateMovement(entt::registry &registry) {
  entt::basic_view villagers = registry.view<Actors::RomanVillager>();

  villagers.each([](Actors::RomanVillager &romanVillager) {
    if (Vector2Distance(romanVillager.destination, romanVillager.position) >
        0.5f) {
      Vector2 foo = {
          romanVillager.destination.x - romanVillager.position.x,
          romanVillager.destination.y - romanVillager.position.y,
      };

      Vector2 unitVec = Vector2Normalize(foo);
      romanVillager.position.x += unitVec.x * romanVillager.speed;
      romanVillager.position.y += unitVec.y * romanVillager.speed;
    }
  });
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
  double relY = y - (row * gridHeight);
  double relX;

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

}; // namespace Actors
