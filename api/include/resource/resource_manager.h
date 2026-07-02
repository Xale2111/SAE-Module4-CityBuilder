//
// Created by alexk on 25.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_SRC_RESOURCE_RESOURCEMANAGER_H_
#define SAE_ALEXK_CITYBUILDER_API_SRC_RESOURCE_RESOURCEMANAGER_H_

#include "SFML/System/Vector2.hpp"
#include "resource/resource.h"
#include <vector>
#include "data_utils.h"
#include <span>

namespace resource {

class ResourceManager {

 public:
  ResourceManager() {
    resource_map_.reserve(DataUtils::kTilemapWidth*DataUtils::kTilemapHeight);
  };
  void AddResource(sf::Vector2i position, ResourcesType type);
  void SetResourceType(int index, ResourcesType newType);

  void Update();

  [[nodiscard]] std::span<Resource> get_resources() {return resource_map_;};

 private:
  std::vector<Resource> resource_map_;
};
}
#endif //SAE_ALEXK_CITYBUILDER_API_SRC_RESOURCE_RESOURCEMANAGER_H_
