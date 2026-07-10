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
#include "building/building_resource.h"

namespace resource {

class ResourceManager {

 public:
  ResourceManager() {
    resource_map_.reserve(DataUtils::kTilemapWidth*DataUtils::kTilemapHeight);
  };
  void AddResource(sf::Vector2i position, ResourcesType type);
  void SetResourceType(int index, ResourcesType newType);

  void Update(float dt);

  void CollectResources(std::span<ResourcesType> collectedResources);
  [[nodiscard]] bool const CheckAvailableResource (std::span<building::BuildingResource> neededResources) const;
  //Overload of function to either give the building resources or directly a specific amount of a resource
  void UseResource(std::span<building::BuildingResource> usedResources);
  void UseResource(int amount, ResourcesType type);



  [[nodiscard]] std::span<Resource> get_resources() {return resource_map_;};
  [[nodiscard]] std::vector<Resource>& get_resources_ref() {return resource_map_;};

  [[nodiscard]] int const get_wood_amount() {return amount_of_collected_wood_;};
  [[nodiscard]] int const get_stone_amount() {return amount_of_collected_stone_;};
  [[nodiscard]] int const get_food_amount() {return amount_of_collected_food_;};


 private:
  std::vector<Resource> resource_map_;
  //TODO : Display the amount of each resource collected
  int amount_of_collected_wood_ = 0;
  int amount_of_collected_stone_= building::kLumberjackNeededStoneAmount.amount_;
  int amount_of_collected_food_ = building::kLumberjackNeededFoodAmount.amount_;
};
}
#endif //SAE_ALEXK_CITYBUILDER_API_SRC_RESOURCE_RESOURCEMANAGER_H_
