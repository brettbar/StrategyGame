#pragma once

#include "../../global.hpp"
#include "element.hpp"

namespace UI {

struct Panel {
  Element elem;
  Axis children_axis;
  Align children_horiz_align;
  Align children_vert_align;
  bool abs_pos = false;
  bool abs_size = false;
  std::function<Vector2()> update_pos;
  std::function<Vector2()> update_size;
  std::vector<entt::entity> children;

  // Absolute-Positioned
  static entt::entity Create(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align,
    bool abs_pos,
    std::function<Vector2()> update_pos,
    std::vector<entt::entity> children
  ) {
    entt::entity entity = Global::local.create();
    Panel panel = Panel(
      id,
      background,
      children_axis,
      children_horiz_align,
      children_vert_align,
      abs_pos,
      false,
      update_pos,
      {},
      children
    );

    Global::local.emplace<Panel>( entity, panel );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  // // Absolute-Sized
  // static entt::entity Create(
  //   std::string id,
  //   Color background,
  //   Axis children_axis,
  //   Align children_horiz_align,
  //   Align children_vert_align,
  //   bool abs_size,
  //   std::function<Vector2()> update_size,
  //   std::vector<entt::entity> children
  // ) {
  //   entt::entity entity = Global::local.create();
  //   Panel panel = Panel(
  //     id,
  //     background,
  //     children_axis,
  //     children_horiz_align,
  //     children_vert_align,
  //     false,
  //     abs_size,
  //     {},
  //     update_size,
  //     children
  //   );

  //   Global::local.emplace<Panel>( entity, panel );
  //   lookup.insert_or_assign( id, entity );
  //   return entity;
  // }

  // Absolute-Sized and Positioned
  static entt::entity Create(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align,
    bool abs_pos,
    bool abs_size,
    std::function<Vector2()> update_pos,
    std::function<Vector2()> update_size,
    std::vector<entt::entity> children
  ) {
    entt::entity entity = Global::local.create();
    Panel panel = Panel(
      id,
      background,
      children_axis,
      children_horiz_align,
      children_vert_align,
      abs_pos,
      abs_size,
      update_pos,
      update_size,
      children
    );

    Global::local.emplace<Panel>( entity, panel );
    lookup.insert_or_assign( id, entity );
    return entity;
  }

  static entt::entity Create(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align,
    std::vector<entt::entity> children
  ) {
    entt::entity entity = Global::local.create();
    Panel panel = Panel(
      id,
      background,
      children_axis,
      children_horiz_align,
      children_vert_align,
      children
    );

    Global::local.emplace<Panel>( entity, panel );
    lookup.insert_or_assign( id, entity );
    return entity;
  }


  void Place() {
    if ( abs_pos ) {
      Vector2 new_pos = update_pos();
      elem.transform.x = new_pos.x;
      elem.transform.y = new_pos.y;
    }
  }

  void Resize() {
    if ( abs_size ) {
      Vector2 new_size = update_size();
      elem.transform.width = new_size.x;
      elem.transform.height = new_size.y;
    }
  }

  void Draw() {
    DrawRectangleV(
      { elem.transform.x, elem.transform.y },
      { elem.transform.width, elem.transform.height },
      elem.background
    );
  }

  private:
  // Absolute panel
  Panel(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align,
    bool absolute_pos,
    bool resizeable,
    std::function<Vector2()> update_pos,
    std::function<Vector2()> update_size,
    std::vector<entt::entity> children
  )
      : elem(
          Element( id, Type::Panel, background, false, { 0, 0, 500, 200 }, {} )
        ),
        children_axis( children_axis ),
        children_horiz_align( children_horiz_align ),
        children_vert_align( children_vert_align ), abs_pos( absolute_pos ),
        abs_size( resizeable ), update_pos( update_pos ),
        update_size( update_size ), children( children ) {
  }

  // Relative panel
  Panel(
    std::string id,
    Color background,
    Axis children_axis,
    Align children_horiz_align,
    Align children_vert_align,
    std::vector<entt::entity> children
  )
      : elem(
          Element( id, Type::Panel, background, false, { 0, 0, 80, 200 }, {} )
        ),
        children_axis( children_axis ),
        children_horiz_align( children_horiz_align ),
        children_vert_align( children_vert_align ), abs_pos( false ),
        abs_size( false ), update_pos( {} ), update_size( {} ),
        children( children ){};
};


};// namespace UI
