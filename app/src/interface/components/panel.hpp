#pragma once

#include "element.hpp"


namespace UI {

  struct Panel : Element {
    Axis children_axis;
    Align children_horiz_align;
    Align children_vert_align;
    bool abs_size = false;
    std::function<void( Panel & )> update;
    std::vector<std::shared_ptr<Element>> children;

    void Update() {
      if ( update )
        update( *this );
    }

    void Draw() {
      DrawRectangleV(
        { transform.x, transform.y },
        { transform.width, transform.height },
        background
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
      std::vector<std::shared_ptr<Element>> children
    )
        : Element( id, background, false, { 0, 0, 500, 200 }, {} ),
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
      std::vector<std::shared_ptr<Element>> children = {}
    )
        : Element( id, background, false, {}, {} ),
          children_axis( children_axis ),
          children_horiz_align( children_horiz_align ),
          children_vert_align( children_vert_align ), update( {} ),
          children( children ){};
    //
    // Relative panel
    Panel(
      std::string id,
      Color background,
      Axis children_axis,
      Align children_horiz_align,
      Align children_vert_align,
      Margins margins,
      std::vector<std::shared_ptr<Element>> children
    )
        : Element( id, background, false, {}, margins ),
          children_axis( children_axis ),
          children_horiz_align( children_horiz_align ),
          children_vert_align( children_vert_align ), update( {} ),
          children( children ){};
  };

  // NOTE For right now, StackPanels can only have Panels as their direct children
  struct StackPanel : Element {
    u32 curr_index = 0;
    // bool abs_pos = false;
    // bool abs_size = false;
    // std::function<Vector2()> update_pos;
    // std::function<Vector2()> update_size;

    // TODO determine if this will always only be a Panel
    // as child
    std::vector<std::shared_ptr<Element>> children;

    std::string ID() {
      return id;
    }

    void Draw() {
      DrawRectangleV(
        { transform.x, transform.y },
        { transform.width, transform.height },
        background
      );
    }

    // Relative panel
    StackPanel(
      std::string id,
      Color background,
      std::vector<std::shared_ptr<Element>> children
    )
        : Element( id, background, false, {}, {} ), children( children ){};
  };

};// namespace UI
