// TODO, this file should be stripped down and converted to a utils file

#pragma once

#include "../include/entt/entt.hpp"
#include <map>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <vector>

using ECS = entt::registry;
using u32 = unsigned int;
using i32 = int;
using f32 = float;
// using str = std::string;
using hstr = entt::hashed_string;
using vec2 = Vector2;
using rect = Rectangle;

template<typename T>
using View = entt::basic_view<
  entt::basic_registry<entt::entity>::entity_type,
  entt::get_t<T>,
  entt::exclude_t<>>;

struct IVector2 {
  i32 x;
  i32 y;
};

struct UVector2 {
  u32 x;
  u32 y;
};

const f32 TILE_WIDTH = 64;
const f32 TILE_HEIGHT = 64;
const u32 MAP_WIDTH = 128;
const u32 MAP_HEIGHT = 128;


inline i32 DetermineTileIdFromPosition( Vector2 );
inline std::unique_ptr<Vector2> DetermineTilePos( Vector2 );

inline void PrintVec2( Vector2 vec ) { printf( "(%f, %f)\n", vec.x, vec.y ); }

inline u32 RollN( u32 n ) { return rand() % n + 1; }

inline void PrintRect( Rectangle rect ) {
  printf(
    "(x:%f, y:%f, w:%f, h:%f)\n",
    rect.x,
    rect.y,
    rect.width,
    rect.height );
}

inline i32 DetermineTileIdFromPosition( Vector2 pos ) {
  std::unique_ptr<Vector2> target = DetermineTilePos( pos );
  if ( target == nullptr )

    return -1;

  i32 row = target->y / 48.0;
  i32 column;

  if ( row % 2 == 1 ) {
    column = ( target->x - 32.0 ) / 64.0;
  } else {
    column = target->x / 64.0;
  }

  return column + row * MAP_WIDTH;
}

inline std::unique_ptr<Vector2> DetermineTilePos( Vector2 inputPos ) {
  i32 x = inputPos.x;
  i32 y = inputPos.y;
  i32 gridHeight = 48;
  i32 gridWidth = 64;
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

  f32 c = 16.0;
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

  f32 tileOrigX = ( column * 64.0 ) + 32.0;
  f32 tileOrigY = ( row * 48.0 ) + 32.0;

  if ( row % 2 == 1 ) {
    tileOrigX += 32.0;
  }
  return std::make_unique<Vector2>( Vector2{ tileOrigX, tileOrigY } );
}

inline std::unique_ptr<UVector2> DetermineTileCoords( Vector2 inputPos ) {
  i32 x = inputPos.x;
  i32 y = inputPos.y;
  i32 gridHeight = 48;
  i32 gridWidth = 64;
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

  f32 c = 16.0;
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

// TODO this probably needs to get moved
inline static void DrawPerfectTexture(
  Texture2D &texture,
  Rectangle rect,
  Vector2 position,
  Color tint ) {
  SetTextureFilter( texture, TEXTURE_FILTER_BILINEAR );

  // SetTextureWrap( texture, TEXTURE_WRAP_MIRROR_REPEAT );
  // SetTextureFilter( texture, TEXTURE_FILTER_POINT );

  DrawTextureRec( texture, rect, position, tint );
}


inline static void
DrawPerfectTexture( Texture2D &texture, Vector2 position, Color tint ) {
  SetTextureFilter( texture, TEXTURE_FILTER_BILINEAR );
  DrawTextureV( texture, position, WHITE );
}


inline static std::string EntityIdToString( entt::entity entity ) {
  if ( entity == entt::null )
    return "-1";
  else
    return std::to_string( (std::uint32_t) entity );
}
