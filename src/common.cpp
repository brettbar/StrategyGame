#include "common.hpp"

void PrintVec2(Vector2 vec) { printf("(%f, %f)\n", vec.x, vec.y); }

i32 determineTileIdFromClick(Vector2 clickPos)
{
  Vector2 *target = determineTilePos(clickPos);
  if (target == nullptr) return -1;

  i32 row = target->y / 96.0;
  i32 column;

  if (row % 2 == 1) { column = (target->x - 64.0) / 128.0; }
  else
  {
    column = target->x / 128.0;
  }

  return column + row * 128;
}

Vector2 *determineTilePos(Vector2 inputPos)
{
  i32 x = inputPos.x;
  i32 y = inputPos.y;
  i32 gridHeight = 96;
  i32 gridWidth = 128;
  i32 halfWidth = gridWidth / 2;

  // Find the row and column of the box that the point falls in.
  int row = (int) (y / gridHeight);
  int column;

  bool rowIsOdd = row % 2 == 1;

  // Is the row an odd number?
  if (rowIsOdd)// Yes: Offset x to match the indent of the row
    column = (int) ((x - halfWidth) / gridWidth);
  else// No: Calculate normally
    column = (int) (x / gridWidth);

  // Work out the position of the point relative to the box it is in
  f32 relY = y - (row * gridHeight);
  f32 relX;

  if (rowIsOdd) relX = (x - (column * gridWidth)) - halfWidth;
  else
    relX = x - (column * gridWidth);

  f32 c = 32.0;
  f32 m = c / halfWidth;

  // Work out if the point is above either of the hexagon's top edges
  if (relY < (-m * relX) + c)
  {// LEFT edge
    row--;
    if (!rowIsOdd) column--;
  }
  else if (relY < (m * relX) - c)
  {// RIGHT edge
    row--;
    if (rowIsOdd) column++;
  }

  f32 tileOrigX = (column * 128.0) + 64.0;
  f32 tileOrigY = (row * 96.0) + 64.0;

  if (row % 2 == 1) { tileOrigX += 64.0; }
  return new Vector2{tileOrigX, tileOrigY};
}
