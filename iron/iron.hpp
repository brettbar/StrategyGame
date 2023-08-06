#include "../app/src/shared/common.hpp"

struct Rect
{
  u32 x;
  u32 y;
  u32 w;
  u32 h;
  Color c;
};

extern "C"
{
  Rect zig_draw();
}
