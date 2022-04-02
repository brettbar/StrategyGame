#pragma once

#include "../include/entt/entt.hpp"
#include "world/systems/player_system.hpp"
#include <map>
#include <raylib.h>
#include <raymath.h>
#include <vector>

using ECS = entt::registry;
using u32 = unsigned int;
using i32 = int;
using f32 = float;
using str = std::string;
using hstr = entt::hashed_string;

struct IVector2 {
  i32 x;
  i32 y;
};

struct UVector2 {
  u32 x;
  u32 y;
};

inline i32 DetermineTileIdFromClick( Vector2 );
inline std::unique_ptr<Vector2> DetermineTilePos( Vector2 );

inline void PrintVec2( Vector2 vec ) { printf( "(%f, %f)\n", vec.x, vec.y ); }

inline void PrintRect( Rectangle rect ) {
  printf(
    "(x:%f, y:%f, w:%f, h:%f)\n",
    rect.x,
    rect.y,
    rect.width,
    rect.height );
}

inline i32 DetermineTileIdFromClick( Vector2 clickPos ) {
  std::unique_ptr<Vector2> target = DetermineTilePos( clickPos );
  if ( target == nullptr )

    return -1;

  i32 row = target->y / 96.0;
  i32 column;

  if ( row % 2 == 1 ) {
    column = ( target->x - 64.0 ) / 128.0;
  } else {
    column = target->x / 128.0;
  }

  return column + row * 128;
}

inline std::unique_ptr<Vector2> DetermineTilePos( Vector2 inputPos ) {
  i32 x = inputPos.x;
  i32 y = inputPos.y;
  i32 gridHeight = 96;
  i32 gridWidth = 128;
  i32 halfWidth = gridWidth / 2;

  // Find the row and column of the box that the point falls in.
  int row = (int) ( y / gridHeight );
  int column;

  bool rowIsOdd = row % 2 == 1;

  // Is the row an odd number?
  if ( rowIsOdd )// Yes: Offset x to match the indent of the row
    column = (int) ( ( x - halfWidth ) / gridWidth );
  else// No: Calculate normally
    column = (int) ( x / gridWidth );

  // Work out the position of the point relative to the box it is in
  f32 relY = y - ( row * gridHeight );
  f32 relX;

  if ( rowIsOdd )

    relX = ( x - ( column * gridWidth ) ) - halfWidth;
  else
    relX = x - ( column * gridWidth );

  f32 c = 32.0;
  f32 m = c / halfWidth;

  // Work out if the point is above either of the hexagon's top edges
  if ( relY < ( -m * relX ) + c ) {// LEFT edge
    row--;
    if ( !rowIsOdd )

      column--;
  } else if ( relY < ( m * relX ) - c ) {// RIGHT edge
    row--;
    if ( rowIsOdd )

      column++;
  }

  f32 tileOrigX = ( column * 128.0 ) + 64.0;
  f32 tileOrigY = ( row * 96.0 ) + 64.0;

  if ( row % 2 == 1 ) {
    tileOrigX += 64.0;
  }
  return std::make_unique<Vector2>( Vector2{ tileOrigX, tileOrigY } );
}

inline std::unique_ptr<UVector2> DetermineTileCoords( Vector2 inputPos ) {
  i32 x = inputPos.x;
  i32 y = inputPos.y;
  i32 gridHeight = 96;
  i32 gridWidth = 128;
  i32 halfWidth = gridWidth / 2;

  // Find the row and column of the box that the point falls in.
  u32 row = (u32) ( y / gridHeight );
  u32 column;

  bool rowIsOdd = row % 2 == 1;

  // Is the row an odd number?
  if ( rowIsOdd )// Yes: Offset x to match the indent of the row
    column = (u32) ( ( x - halfWidth ) / gridWidth );
  else// No: Calculate normally
    column = (u32) ( x / gridWidth );

  // Work out the position of the point relative to the box it is in
  f32 relY = y - ( row * gridHeight );
  f32 relX;

  if ( rowIsOdd )

    relX = ( x - ( column * gridWidth ) ) - halfWidth;
  else
    relX = x - ( column * gridWidth );

  f32 c = 32.0;
  f32 m = c / halfWidth;

  // Work out if the point is above either of the hexagon's top edges
  if ( relY < ( -m * relX ) + c ) {// LEFT edge
    row--;
    if ( !rowIsOdd )

      column--;
  } else if ( relY < ( m * relX ) - c ) {// RIGHT edge
    row--;
    if ( rowIsOdd )

      column++;
  }

  return std::make_unique<UVector2>( UVector2{ column, row } );

  //  f32 tileOrigX = (column * 128.0) + 64.0;
  //  f32 tileOrigY = (row * 96.0) + 64.0;
  //
  //  if (row % 2 == 1) {
  //    tileOrigX += 64.0;
  //  }
}
