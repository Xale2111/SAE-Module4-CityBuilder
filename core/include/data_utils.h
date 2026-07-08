//
// Created by alexk on 28.05.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_CORE_INCLUDE_DATA_UTILS_H_
#define SAE_ALEXK_CITYBUILDER_CORE_INCLUDE_DATA_UTILS_H_
enum class ActionCode {
  kPlay,
  kMenu,
  kPause,
  kResume,
  kQuit
};

enum class ResourcesType {
  kWood,
  kStone,
  kFood,
  kNone
};
namespace resource{
constexpr float kTreeGrowingTime = 90.f;
constexpr float kStoneGrowingTime = 150.f;
constexpr float kFoodGrowingTime = 120.f;
}

enum class ResourceState {
  kReady,
  kOccupied,
  kGrowing,
};

enum class DisplayableBuilding {
  kFoodHouse,
  kLumberjackHouse,
  kMineHouse,
  kCanteen,
  kNone
};

enum class NpcType {
  kGatherer,
  kLumberjack,
  kMiner,
  kNone
};

enum class PathRequest {
  kNone,
  kResource,
  kHome
};

enum class PathStatus{
  kFound,
  kSearching,
  kNoPath,
  kNone,
};

struct DataUtils {
  static constexpr int kScreenWidth = 1920;
  static constexpr int kScreenHeight = 1080;

  //wanted size is 400x400
  static constexpr int kTilemapWidth = 400;
  static constexpr int kTilemapHeight = 400;
  static constexpr int kTileSize = 64;

};
#endif //SAE_ALEXK_CITYBUILDER_CORE_INCLUDE_DATA_UTILS_H_
