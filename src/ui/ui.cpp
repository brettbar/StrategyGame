#include "ui.hpp"
#include <raylib.h>

namespace UI
{

  void Create(State &state, entt::registry &reg, TextureCache &cache)
  {
    auto sidebarEntt = reg.create();

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

    reg.emplace<PanelSolid>(sidebarEntt, sidebar);
  }

  void Update(State &state, entt::registry &reg)
  {
    auto elemView = reg.view<PanelSolid>();


    for (auto &el: elemView)
    {
      PanelSolid &sidebar = elemView.get<PanelSolid>(el);
      sidebar.layout = {
        .width = state.screenWidth / 12,
        .height = state.screenHeight,
      };
    }
  }


  void Draw(State &state, entt::registry &reg)
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
      if ((u32)prov.owner == state.currPlayer->id) {
        DrawRectangleV(prov.tile->position, {256, 256}, RED);
      }

    }

    DrawRectangle(0, 0, (f32) state.screenWidth, 24.0f, BLACK);
    DrawFPS(16, 2);
    DrawText(state.currPlayer->factionName.c_str(), 720, 7, 14, BLUE);

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

    DrawText(symbol.c_str(), state.screenWidth - 128, 2,
             20, WHITE);
  }

};// namespace UI
