#pragma once

#include "common.hpp"
#include <memory>

inline i32 DetermineTileIdFromPosition( Vector2 );
inline std::unique_ptr<Vector2> DetermineTilePos( Vector2 );
inline std::unique_ptr<UVector2> DetermineTileCoords( Vector2 );
inline std::unique_ptr<Vector2> DeterminePosFromTileCoords( UVector2 );

inline long TimestampMS()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(
           std::chrono::system_clock::now().time_since_epoch()
  )
    .count();
}

inline void CameraUpdate( Camera2D &camera, f32 dt )
{
  f32 cameraSpeed = 500.0f;
  // Vector2 screenCenter = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  // Vector2 target = GetScreenToWorld2D(screenCenter, camera);
  // PrintVec2(target);

  // camera.offset = target;

  if ( IsKeyDown( KEY_D ) )
    camera.target.x += dt * cameraSpeed / camera.zoom;
  if ( IsKeyDown( KEY_A ) )
    camera.target.x -= dt * cameraSpeed / camera.zoom;
  if ( IsKeyDown( KEY_W ) )
    camera.target.y -= dt * cameraSpeed / camera.zoom;
  if ( IsKeyDown( KEY_S ) )
    camera.target.y += dt * cameraSpeed / camera.zoom;

  if ( IsKeyDown( KEY_Z ) )
    camera.zoom -= 0.05f;
  if ( IsKeyDown( KEY_X ) )
    camera.zoom += 0.05f;

  f32 mouseWheelDelta = GetMouseWheelMove();

  camera.zoom += ( mouseWheelDelta * 0.2f );
  if ( camera.zoom > 8.0f )
    camera.zoom = 8.0f;
  else if ( camera.zoom < 0.08f )
    camera.zoom = 0.08f;

  camera.offset = { (f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2 };
}

inline void PrintVec2( Vector2 vec )
{
  printf( "(%f, %f)\n", vec.x, vec.y );
}
inline u32 RollN( u32 n )
{
  return rand() % n + 1;
}

inline void PrintRect( Rectangle rect )
{
  printf(
    "(x:%f, y:%f, w:%f, h:%f)\n", rect.x, rect.y, rect.width, rect.height
  );
}

inline i32 DetermineTileIdFromPosition( Vector2 pos )
{
  std::unique_ptr<Vector2> target = DetermineTilePos( pos );
  if ( target == nullptr )

    return -1;

  i32 row = target->y / 48.0;
  i32 column;

  if ( row % 2 == 1 )
  {
    column = ( target->x - 32.0 ) / 64.0;
  }
  else
  {
    column = target->x / 64.0;
  }

  return column + row * MAP_WIDTH;
}

inline std::unique_ptr<Vector2> DetermineTilePos( Vector2 inputPos )
{
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
  if ( relY < ( -m * relX ) + c )
  {// LEFT edge
    row--;
    if ( !rowIsOdd )

      column--;
  }
  else if ( relY < ( m * relX ) - c )
  {// RIGHT edge
    row--;
    if ( rowIsOdd )

      column++;
  }

  f32 tileOrigX = ( column * 64.0 ) + 32.0;
  f32 tileOrigY = ( row * 48.0 ) + 32.0;

  if ( row % 2 == 1 )
  {
    tileOrigX += 32.0;
  }
  return std::make_unique<Vector2>( Vector2{ tileOrigX, tileOrigY } );
}

inline std::unique_ptr<UVector2> DetermineTileCoords( Vector2 inputPos )
{
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
  if ( relY < ( -m * relX ) + c )
  {// LEFT edge
    row--;
    if ( !rowIsOdd )

      column--;
  }
  else if ( relY < ( m * relX ) - c )
  {// RIGHT edge
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


// inline static void
// DrawPerfectTexture( Texture2D &texture, Vector2 position, Color tint ) {
//   // SetTextureFilter( texture, TEXTURE_FILTER_BILINEAR );
//   DrawTextureV( texture, position, WHITE );
// }

// inline static void DrawPerfectTexture(
//   Texture2D &texture,
//   Rectangle rect,
//   Vector2 position,
//   Color tint ) {
//   // SetTextureFilter( texture, TEXTURE_FILTER_BILINEAR );

//   // SetTextureWrap( texture, TEXTURE_WRAP_MIRROR_REPEAT );
//   // SetTextureFilter( texture, TEXTURE_FILTER_POINT );

//   DrawTextureRec( texture, rect, position, tint );
// }

// TODO(rf) this should be from json not hardcoded like this
inline Color GetPrimaryFactionColor( std::string faction )
{
  if ( faction == "romans" )
    return RED;
  if ( faction == "greeks" )
    return BLUE;
  if ( faction == "celts" )
    return GREEN;
  if ( faction == "punics" )
    return PURPLE;
  if ( faction == "germans" )
    return GRAY;
  if ( faction == "scythians" )
    return PINK;
  if ( faction == "persians" )
    return ORANGE;
  else
    return BLACK;
}

inline std::string FormatRGB( Color color )
{
  return "r: " + std::to_string( color.r ) +
         " g: " + std::to_string( color.g ) +
         " b: " + std::to_string( color.b );
}

inline static std::string EntityIdToString( entt::entity entity )
{
  if ( entity == entt::null )
    return "-1";
  else
    return std::to_string( (std::uint32_t) entity );
}
