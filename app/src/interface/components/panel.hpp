#pragma once

#include "element.hpp"

namespace UI {

  // TODO replace update_pos, update_size with one update fn
  struct Panel {
    Element elem;
    Axis children_axis;
    Align children_horiz_align;
    Align children_vert_align;
    std::function<void( Panel & )> update;
    std::vector<entt::entity> children;

    // With update
    static entt::entity CreateDynamic(
      std::string id,
      Color background,
      Axis children_axis,
      Align children_horiz_align,
      Align children_vert_align,
      std::function<void( Panel & )> update,
      std::vector<entt::entity> children
    ) {
      entt::entity entity = Global::local.create();
      Panel panel = Panel(
        id,
        background,
        children_axis,
        children_horiz_align,
        children_vert_align,
        true,
        update,
        children
      );

      Global::local.emplace<Panel>( entity, panel );
      lookup.insert_or_assign( id, entity );
      return entity;
    }

    // Default Panel
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


    // void Place() {
    //   if ( abs_pos ) {
    //     Vector2 new_pos = update();
    //     elem.transform.x = new_pos.x;
    //     elem.transform.y = new_pos.y;
    //   }
    // }

    // void Resize() {
    //   if ( abs_size ) {
    //     Vector2 new_size = update();
    //     elem.transform.width = new_size.x;
    //     elem.transform.height = new_size.y;
    //   }
    // }
    void Update() {
      if ( update )
        update( *this );
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
      bool absolute,
      std::function<void( Panel & )> update,
      std::vector<entt::entity> children
    )
        : elem( Element(
            id,
            Type::Panel,
            background,
            false,
            { 0, 0, 500, 200 },
            {}
          ) ),
          children_axis( children_axis ),
          children_horiz_align( children_horiz_align ),
          children_vert_align( children_vert_align ), update( update ),
          children( children ) {}

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
          children_vert_align( children_vert_align ), update( {} ),
          children( children ){};
  };

  ////
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
