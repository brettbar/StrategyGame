#include "../app/src/shared/common.hpp"
#include <raylib.h>

struct DrawCall
{
  u32 tag;
  Rectangle rect;
  Color bg;
};

extern "C"
{
  DrawCall zig_draw();
}
