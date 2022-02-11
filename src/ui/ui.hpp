#pragma once

#include "../common.hpp"
#include "../resource.hpp"
#include "../state.hpp"
#include "../systems/map/provinces.hpp"

namespace UI
{
  enum Style
  {
    RECT_FILLED,
    RECT_LINES,
    RECT_ROUNDED,
  };

  enum LayoutStyle
  {
    HORIZONTAL,
    VERTICAL,
    GRID,
  };

  struct Element {
    u32 id;
    str name;
    bool debug;
    bool stateful;
    Vector2 position;
  };

  struct Layout {
    u32 width;
    u32 height;
    LayoutStyle style;
  };

  struct PanelSolid : Element {
    Style style;
    Color color;
    Layout layout;
    std::vector<Element> children;
  };

  struct PanelTexture : Element {
    Texture texture;
    Layout layout;
    std::vector<Element> children;
  };

  struct TextButton : Element {
    Style style;
    Color color;
    str text;
  };

  struct ImageButton : Element {
    Texture2D texture;
  };

  inline void Update(State &, entt::registry &);
  inline void Draw(State &, entt::registry &);
  inline str FormatDate(u32, u32, u32);

  inline void Create(State &state, entt::registry &reg, TextureCache &cache)
  {
    PanelSolid sidebar = PanelSolid();
    sidebar.id = 0;
    sidebar.name = "Sidebar";
    sidebar.debug = false;
    sidebar.stateful = true;
    sidebar.position = {0, 0};
    sidebar.style = RECT_FILLED;
    sidebar.color = BLACK;
    sidebar.layout = {
      .width = state.screenWidth / 12,
      .height = state.screenHeight,
    };
    sidebar.children = {};

    PanelSolid bottom = PanelSolid();
    bottom.id = 1;
    bottom.name = "Bottom";
    bottom.debug = false;
    bottom.stateful = true;
    bottom.style = RECT_FILLED;
    bottom.color = BLACK;
    bottom.position = {
      (f32) state.screenWidth / 4, (f32) state.screenHeight - state.screenHeight / 6.0f};
    bottom.layout = {
      .width = state.screenWidth / 2,
      .height = state.screenHeight / 6,
    };
    bottom.children = {};

    reg.emplace<PanelSolid>(reg.create(), sidebar);
    reg.emplace<PanelSolid>(reg.create(), bottom);
  }

  inline void Update(State &state, entt::registry &reg)
  {
    auto elemView = reg.view<PanelSolid>();

    for (auto &el: elemView)
    {
      PanelSolid &panel = elemView.get<PanelSolid>(el);
      switch (panel.id)
      {
        case 0:
          panel.layout = {
            .width = state.screenWidth / 12,
            .height = state.screenHeight,
          };
          break;
        case 1:
          panel.position = {
            (f32) state.screenWidth / 4, (f32) state.screenHeight - state.screenHeight / 6.0f};
          panel.layout = {
            .width = state.screenWidth / 2,
            .height = state.screenHeight / 6,
          };
          break;
        default:
          break;
      }
    }
  }

  inline void Draw(State &state, entt::registry &reg)
  {
    auto elView = reg.view<PanelSolid>();
    auto provView = reg.view<Provinces::Province>();

    for (auto &el: elView)
    {
      PanelSolid element = elView.get<PanelSolid>(el);
      DrawRectangleV(element.position, {(f32) element.layout.width, (f32) element.layout.height}, element.color);
    }

    for (auto &provEntt: provView)
    {
      Provinces::Province &prov = provView.get<Provinces::Province>(provEntt);
      if ((u32) prov.owner == state.currPlayer->id)
      {
        DrawRectangleV(prov.tile->position, {256, 256}, RED);
      }
    }

    DrawRectangle(0, 0, (f32) state.screenWidth, 24.0f, BLACK);
    DrawFPS(16, 2);
    DrawText(state.currPlayer->factionName.c_str(), GetScreenWidth() / 2, 7, 14, BLUE);
    DrawText(FormatDate(state.month, state.year, state.startYear).c_str(),
             state.screenWidth - 128, 7, 14, GREEN);

    str symbol = "||";
    if (state.timeScale == 1.5)
    {
      symbol = ">>>";
    }
    else if (state.timeScale == 1.0)
    {
      symbol = ">>";
    }
    else if (state.timeScale == 0.5)
    {
      symbol = ">";
    }
    else
    {
      symbol = "||";
    }

    DrawText(symbol.c_str(), state.screenWidth - 256, 2, 20, WHITE);
  }

  inline str FormatDate(u32 month, u32 year, u32 startYr)
  {
    str months[] = {
      "Jan", "Feb", "Mar", "Apr", "May", "Jun",
      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    if (month > 12) month = 12;
    if (month < 1) month = 1;

    str yr = "";
    if (year >= startYr * 2)
      yr = std::to_string(year - (startYr * 2) + 1) + " AD";
    else
      yr = std::to_string(startYr + (startYr - year)) + " BC";


    return months[month - 1] + ", " + yr;
  }


};// namespace UI
