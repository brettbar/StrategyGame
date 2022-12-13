#pragma once

#include "../../global.hpp"
#include "element.hpp"

namespace UI {

// A stack panel is a panel that only ever has one
// active child.
struct StackPanel {
  Element elem;
  u32 curr_index = 0;
  // bool abs_pos = false;
  // bool abs_size = false;
  // std::function<Vector2()> update_pos;
  // std::function<Vector2()> update_size;
  std::vector<entt::entity> children;

  static entt::entity Create(
    std::string id,
    Color background,
    std::vector<entt::entity> children
  ) {
    entt::entity entity = Global::local.create();
    StackPanel panel = StackPanel( id, background, children );

    Global::local.emplace<StackPanel>( entity, panel );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  void Draw() {
    DrawRectangleV(
      { elem.transform.x, elem.transform.y },
      { elem.transform.width, elem.transform.height },
      elem.background
    );
  }

  private:
  // Relative panel
  StackPanel(
    std::string id,
    Color background,
    std::vector<entt::entity> children
  )
      : elem( Element(
          id,
          Type::StackPanel,
          background,
          false,
          { 0, 0, 80, 200 },
          {}
        ) ),
        children( children ){};
};

};// namespace UI
