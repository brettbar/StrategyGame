/*
  Authored by Brett Barinaga on 11/29/21. Copyright (c) Brett Barinaga, All
rights reserved.
*/


#include "shared/state.hpp"
#include "ui/common.h"
#include <raylib.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"

#include "shared/common.hpp"
#include "shared/global.hpp"
#include "steam/steam_api.h"
#include "steam/steam_api_common.h"
#include <nlohmann/json.hpp>
#include <stdlib.h>

#include "assets.hpp"
#include "game.hpp"
#include "network/network.hpp"
#include "settings.hpp"


void SteamAPIDebugTextHook(int severity, const char *msg) {
  printf("S::%d", severity);
  printf("%s\n", msg);
}

void HandleClayErrors(Clay_ErrorData errorData) {
  printf("%s\n", errorData.errorText.chars);
  if (errorData.errorType == CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED) {
    // reinitializeClay = true;
    Clay_SetMaxElementCount(Clay_GetMaxElementCount() * 2);
  } else if (errorData.errorType == CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED) {
    // reinitializeClay = true;
    Clay_SetMaxMeasureTextCacheWordCount(
      Clay_GetMaxMeasureTextCacheWordCount() * 2
    );
  }
}

void scale_ui_with_screen_size() {
  u32 monitor_count = GetMonitorCount();

  u32 monitor = 0;
  u32 monitor_w = GetMonitorWidth(monitor);
  u32 monitor_h = GetMonitorHeight(monitor);
  u32 physical_w = GetMonitorPhysicalWidth(monitor);
  u32 physical_h = GetMonitorPhysicalHeight(monitor);

  f32 height_dpi = monitor_h / (physical_h / 25.4f);

  printf(
    "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDd %f\n",
    height_dpi
  );

  UI::set_ui_scale_by_screen_height(monitor_h);
}

void init_clay(vec2i initial_resolution) {
  u64 clay_required_memory = Clay_MinMemorySize();
  Clay_Arena clay_memory = Clay_Arena{
    .capacity = clay_required_memory,
    .memory = (char *) malloc(clay_required_memory),
  };
  Clay_Initialize(
    clay_memory,
    Clay_Dimensions{
      .width = (f32) initial_resolution.x,
      .height = (f32) initial_resolution.y,
    },
    Clay_ErrorHandler{HandleClayErrors, 0}
  );
}

/*
========================================================
  Main application entrypoint
========================================================
*/
int main() {
  Global::mp_capable = true;

  if (SteamAPI_RestartAppIfNecessary(2296090)) {
    Global::mp_capable = false;
  }

  if (!SteamAPI_Init()) {
    printf("SteamAPI_Init() failed!\n");
    Global::mp_capable = false;
  }

  SteamClient()->SetWarningMessageHook(&SteamAPIDebugTextHook);

  if (!SteamUser()->BLoggedOn()) {
    printf("Steam user is not logged in\n");
    Global::mp_capable = false;
  }

  if (!SteamInput()->Init(false)) {
    printf("SteamInput()->Init failed.\n");
    Global::mp_capable = false;
  }


  printf("Starting game as %s.\n", SteamFriends()->GetPersonaName());

  Settings::Manager()->set_window_state(Settings::WindowState::Window);
  Settings::Manager()->limit_fps(300);

  vec2i set_resolution = Settings::Manager()->resolution();
  InitWindow(set_resolution.x, set_resolution.y, "FieldsOfMars");

  scale_ui_with_screen_size();

  init_clay(set_resolution);

  LoadAssets();

  Global::fonts.push_back(Global::font_cache[hstr{"font_default"}]->font);

  Clay_SetMeasureTextFunction(Raylib_MeasureText, Global::fonts.data());

  SetExitKey(KEY_NULL);

  Network::Setup();

  // This call will block and run until the game exists
  Game()->MainLoop();

  CloseWindow();// Close window and OpenGL context

  // Perform clean up and teardown
  // @TODO figure out all deallocs or whatever
  Renderer::Get()->Shutdown();

  SteamAPI_Shutdown();

  return EXIT_SUCCESS;
}
