#pragma once

#include "../../global.hpp"
#include "../ui_lookups.hpp"

namespace UI {

inline f32 SCALE = 2.0f;
inline std::map<std::string, entt::entity> lookup;

struct Context {
  entt::entity hot;
  entt::entity active;
};

typedef struct Element Element;
typedef struct Panel Panel;
typedef struct TextLabel TextLabel;
typedef struct TextButton TextButton;
typedef struct TextureLabel TextureLabel;
typedef struct TextureButton TextureButton;

enum class Type {
  INVALID_TYPE,
  Panel,
  TextLabel,
  TextureLabel,
  TextButton,
  TextureButton,
};

enum class Axis {
  ROW,
  COLUMN,
};

enum class Align {
  START,
  CENTER,
  END,
  SPACE_OUT,
};

struct Margins {
  u32 left;
  u32 right;
  u32 top;
  u32 bottom;
};

struct Element {
  std::string id;
  Type type;
  Color background;
  bool enabled;
  rect transform;
  Margins margins;

  Element(
    std::string id,
    Type type,
    Color background,
    bool enabled,
    rect transform,
    Margins margins
  )
      : id( id ), type( type ), background( background ), enabled( enabled ),
        transform( transform ), margins( margins ) {
  }
};

template<typename T>
static bool Has( entt::entity entity ) {
  return Global::local.all_of<T>( entity );
}

template<typename T>
static T &Get( entt::entity entity ) {
  assert( Has<T>( entity ) );
  auto &got = Global::local.get<T>( entity );
  return got;
}


};// namespace UI
