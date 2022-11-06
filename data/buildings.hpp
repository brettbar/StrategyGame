#pragma once


namespace Buildings {

// TODO consolidate buildings to one building that can make different things
enum class Gathering {
  WoodCutter,
  WheatFarm,
  OliveGrove,
  FishingDock,//  Wharf, Staith, Jetties, Quay?
  Mine,
  SheepRanch,
  FlaxFarm,
};

enum class Refining {
  SawMill,
  Mill,
  Ovens,// TODO think of better name for a building that can make bread or ceramics
  Weaver,
  Tanner,
  Smelter,
};

enum class Production {
  Tailor,

};

};// namespace Buildings
