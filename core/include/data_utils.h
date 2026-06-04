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
};

 class DataUtils {
 public :
  static constexpr int kScreenWidth = 1920;
  static constexpr int kScreenHeight = 1080;

  static constexpr int kTilemapWidth = 15000;
  static constexpr int kTilemapHeight = 15000;

};
#endif //SAE_ALEXK_CITYBUILDER_CORE_INCLUDE_DATA_UTILS_H_
