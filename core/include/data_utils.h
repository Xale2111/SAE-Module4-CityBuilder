//
// Created by alexk on 28.05.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_CORE_INCLUDE_DATA_UTILS_H_
#define SAE_ALEXK_CITYBUILDER_CORE_INCLUDE_DATA_UTILS_H_
enum class ActionCode : uint8_t {
  kPlay,
  kMenu,
  kPause,
  kResume,
  kQuit
};

enum class ResourcesType : uint8_t {
  kWood,
  kStone,
  kFood,
};

enum class DisplayableBuilding : uint8_t {
  kFoodHouse,
  kLumberjackHouse,
  kMineHouse,
  kCanteen,
  kNone
};

enum class NpcType{
  kGatherer,
  kLumberjack,
  kMiner,
};

class DataUtils {
 public :
  static constexpr int kScreenWidth = 1920;
  static constexpr int kScreenHeight = 1080;

  //wanted size is 400x400
  static constexpr int kTilemapWidth = 400;
  static constexpr int kTilemapHeight = 400;
  static constexpr int kTileSize = 64;

};
#endif //SAE_ALEXK_CITYBUILDER_CORE_INCLUDE_DATA_UTILS_H_
