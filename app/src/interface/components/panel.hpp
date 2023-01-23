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

  // NOTE For right now, StackPanels can only have Panels as their direct children
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

  // NOTE For right now, GridPanels can only have Panels as their direct children
  // Those panels must be relatively sized/positioned
  struct GridPanel {
    Element elem;
    const u32 grid_width;
    const u32 grid_height;
    std::function<void( GridPanel & )> update;
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

    void Update() {
      if ( update )
        update( *this );
    }

    GridPanel(
      std::string id,
      Color background,
      const u32 grid_width,
      const u32 grid_height,
      std::function<void( GridPanel & )> update,
      std::vector<entt::entity> children
    )
        : elem( Element( id, Type::GridPanel, background, false, {}, {} ) ),
          grid_width( grid_width ), grid_height( grid_height ),
          update( update ), children( children ){};
  };

};// namespace UI
