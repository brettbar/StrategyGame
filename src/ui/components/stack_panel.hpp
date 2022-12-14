#pragma once

#include "../../global.hpp"
#include "../ui_lookups.hpp"
#include "../ui_utils.hpp"
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

  // TODO determine if this will always only be a Panel
  // as child
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

  void SwitchChild( u32 index ) {
    ToggleElem( children[curr_index], false );
    curr_index = index;
    ToggleElem( children[curr_index], true );
  }

  private:
  // Relative panel
  StackPanel(
    std::string id,
    Color background,
    std::vector<entt::entity> children
  )
      : elem( Element( id, Type::StackPanel, background, false, {}, {} ) ),
        children( children ){};
};

};// namespace UI
