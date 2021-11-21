#include "map.h"
  
namespace Map {

struct Tile {
  u32 id;
  Vector2 position;
};

void Create(entt::registry &registry, u32 mapWidth, u32 mapHeight) {
  u32 currId = 0;
  Vector2 position = {0.0f, 0.0f};

  for (u32 y = 0; y < mapHeight; y++) {
    for (u32 x = 0; x < mapWidth; x++) {
      entt::entity entity = registry.create();
      Tile tile = Tile{};
      tile.id = currId++;

      if (y % 2 == 1)
        tile.position = {position.x + x * 128 + 64, position.y + y * 96};
      else
        tile.position = {position.x + x * 128, position.y + y * 96};

      registry.emplace<Tile>(entity, tile.id, tile.position);
    }
  }
}

void Draw(entt::registry &registry, Texture2D hex, Rectangle frameRec) {
  auto tiles = registry.view<Tile>();

  tiles.each([hex, frameRec](Tile tile) {
    DrawTextureRec(hex, frameRec, tile.position, WHITE);
  });
}


}; // namespace Map
