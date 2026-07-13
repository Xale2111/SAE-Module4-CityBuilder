//
// Created by alexk on 13.07.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_INCLUDE_SAVER_SAVER_H_
#define SAE_ALEXK_CITYBUILDER_API_INCLUDE_SAVER_SAVER_H_

#include <span>
#include <string>
#include <vector>
#include "data_utils.h"
#include <nlohmann/json.hpp>


namespace resource {
class Resource;
}

namespace saver {
class Saver {
 public :
  void SaveGame(ResourceAmounts playerResources,
                std::span<const resource::Resource> resourceMap,
                std::span<const BuildingInfos> placedBuildings,
                const std::string &filename = "savegame.json");

  bool LoadGame(ResourceAmounts &playerResources,
                std::vector<resource::Resource> &resourceMap,
                std::vector<BuildingInfos> &placedBuildings,
                const std::string &filename = "savegame.json");

 private:
  nlohmann::json SerializePlayerResources(ResourceAmounts playerResources);
  nlohmann::json SerializeWorldResources(std::span<const resource::Resource> resourceMap);
  nlohmann::json SerializeBuildings(std::span<const BuildingInfos> placedBuildings);

  void DeserializePlayerResources(ResourceAmounts &playerResources, const nlohmann::json &json);
  void DeserializeWorldResources(std::vector<resource::Resource> &resourceMap, const nlohmann::json &json);
  void DeserializeBuildings(std::vector<BuildingInfos> &placedBuildings, const nlohmann::json &json);

};
}

#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_SAVER_SAVER_H_
