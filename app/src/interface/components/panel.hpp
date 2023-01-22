#pragma once

#include "element.hpp"

#include "../ui_manager.hpp"

namespace UI {

  struct Panel {
    Element elem;
    Axis children_axis;
    Align children_horiz_align;
    Align children_vert_align;
    bool abs_size = false;
    std::function<void( Panel & )> update;
    std::vector<entt::entity> children;

    std::string ID() {
      return elem.id;
    }

    Type Type() {
      return elem.Type();
    }

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

    // Absolute panel
    Panel(
      std::string id,
      Color background,
      Axis children_axis,
      Align children_horiz_align,
      Align children_vert_align,
      bool abs_size,
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
          children_vert_align( children_vert_align ), abs_size( abs_size ),
          update( update ), children( children ) {}

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

    std::string ID() {
      return elem.id;
    }

    Type Type() {
      return elem.Type();
    }

    void Draw() {
      DrawRectangleV(
        { elem.transform.x, elem.transform.y },
        { elem.transform.width, elem.transform.height },
        elem.background
      );
    }

    // Relative panel
    StackPanel(
      std::string id,
      Color background,
      std::vector<entt::entity> children
    )
        : elem( Element( id, Type::StackPanel, background, false, {}, {} ) ),
          children( children ){};
  };

  struct GridPanel {
    Element elem;
    u32 grid_width;
    u32 grid_height;
    std::function<void( Panel & )> update;
    std::vector<entt::entity> children;

    std::string ID() {
      return elem.id;
    }

    Type Type() {
      return elem.Type();
    }

    void Draw() {
      DrawRectangleV(
        { elem.transform.x, elem.transform.y },
        { elem.transform.width, elem.transform.height },
        elem.background
      );
    }

    GridPanel(
      std::string id,
      Color background,
      std::function<void( Panel & )> update,
      std::vector<entt::entity> children
    )
        : elem( Element( id, Type::GridPanel, background, false, {}, {} ) ),
          children( children ){};
  };

};// namespace UI
