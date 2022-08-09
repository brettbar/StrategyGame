#include "../../../common.hpp"

namespace IRONGUI {

enum class AlignAxis {
  ROW,// Default
  COLUMN,
};

enum class SelfAlign {
  AUTO,// Default
  START,
  END,
  CENTER,
  STRETCH,
};

enum class Alignment {
  START,// Default
  END,
  CENTER,
  SPACE_BETWEEN,
  SPACE_AROUND,
  SPACE_EVENLY,
  STRETCH
};

enum class Type {
  PANEL,
  TEXT_LABEL,
  TEXTURE_LABEL,
  TEXT_BUTTON,
  TEXTURE_BUTTON,
};

struct Margins {
  f32 top;
  f32 right;
  f32 bottom;
  f32 left;
};

struct Element {
  Type type;
  bool enabled;
  Color color;
  Vector2 pos;
  Vector2 dmns;

  Margins margins;

  SelfAlign align_self;
};

struct Panel {
  bool pos_absolute = false;
  AlignAxis children_align_axis;
  Alignment children_main_align;
  Alignment children_cross_align;
  std::vector<entt::entity> children;
};

struct TextLabel {
  // const char *text;
  std::string text;
  i32 font_size;
  Color text_color;
  // Alignment text_align;
};

struct TextButton {
  TextLabel label;
  std::function<void()> action;
};

struct TextureLabel {
  Texture2D texture;
};

inline Rectangle RectangleFromVectors( Vector2 pos, Vector2 dimensions ) {
  return { pos.x, pos.y, dimensions.x, dimensions.y };
}


};// namespace IRONGUI
