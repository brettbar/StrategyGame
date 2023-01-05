#pragma once

#include "shared/common.hpp"
#include "shared/global.hpp"

class GameState {
  public:
  const f32 MS_PER_UPDATE = 1 / 60.0;
  const f32 ONCE_A_SECOND = 1;
  f32 oncelag = 0.0f;
  f32 lag = 0.0f;
  f32 dt = 0.0f;

  bool GameShouldRun() {
    return !hit_exit;
  }

  void HostCampaign() {
    ExitCampaignIfRunning();

    // mult init

    // TODO be able to host exiting campaign
    StartCampaign();
  }

  void JoinCampaign() {
    ExitCampaignIfRunning();
    // mult init

    // TODO be able to host exiting campaign
    StartCampaign();
  }

  void ExitCampaignIfRunning() {
    if ( _in_campaign )
      ExitCampaign();
  }

  void ExitCampaign() {
    Global::ClearRegistry();
    Global::program_mode = Global::ProgramMode::MainMenu;
  }

  void StartCampaign() {
    Global::program_mode = Global::ProgramMode::Campaign;
    UI::EnableCampaignUI();
  }

  void LoadCampaign() {
    SaveSystem::Load();

    StartCampaign();
  }


  private:
  bool _in_campaign = false;
  bool _pending_new_campaign = false;
  bool _pending_load_campaign = false;

  bool hit_exit = false;

  bool creating_lobby = false;
  bool joining_lobby = false;
  // TODO could cause issues by not getting reset
  bool is_host = false;
};

// TODO this is a weird spot to put this, but both game and campaign use it? maybe refactor
inline void CameraUpdate( Camera2D &camera, f32 dt ) {
  f32 cameraSpeed = 500.0f;
  // Vector2 screenCenter = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  // Vector2 target = GetScreenToWorld2D(screenCenter, camera);
  // PrintVec2(target);

  // camera.offset = target;

  if ( IsKeyDown( KEY_D ) )
    camera.target.x += dt * cameraSpeed / camera.zoom;
  if ( IsKeyDown( KEY_A ) )
    camera.target.x -= dt * cameraSpeed / camera.zoom;
  if ( IsKeyDown( KEY_W ) )
    camera.target.y -= dt * cameraSpeed / camera.zoom;
  if ( IsKeyDown( KEY_S ) )
    camera.target.y += dt * cameraSpeed / camera.zoom;

  if ( IsKeyDown( KEY_Z ) )
    camera.zoom -= 0.05f;
  if ( IsKeyDown( KEY_X ) )
    camera.zoom += 0.05f;

  f32 mouseWheelDelta = GetMouseWheelMove();

  camera.zoom += ( mouseWheelDelta * 0.2f );
  if ( camera.zoom > 8.0f )
    camera.zoom = 8.0f;
  else if ( camera.zoom < 0.08f )
    camera.zoom = 0.08f;

  camera.offset = { (f32) GetScreenWidth() / 2, (f32) GetScreenHeight() / 2 };
}
