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

  [[nodiscard]] std::span<const Resource> get_resources() {return resource_map_;};
  [[nodiscard]] std::vector<Resource>& get_resources_ref() {return resource_map_;};

  [[nodiscard]] int get_wood_amount() const {return amount_of_collected_wood_;};
  [[nodiscard]] int get_stone_amount() const {return amount_of_collected_stone_;};
  [[nodiscard]] int get_food_amount() const {return amount_of_collected_food_;};
  [[nodiscard]] int& get_wood_amount_ref() {return amount_of_collected_wood_;};
  [[nodiscard]] int& get_stone_amount_ref() {return amount_of_collected_stone_;};
  [[nodiscard]] int& get_food_amount_ref() {return amount_of_collected_food_;};
  [[nodiscard]] ResourceAmounts get_all_resources_amount() const {return {amount_of_collected_wood_,amount_of_collected_stone_,amount_of_collected_food_};};

  [[nodiscard]] std::vector<int> PopHarvestedResources() {
    std::vector<int> result = std::move(harvested_this_frame_);
    harvested_this_frame_.clear();
    return result;
  }

  [[nodiscard]] std::vector<int> PopRespawnedResources() {
    std::vector<int> result = std::move(respawned_this_frame_);
    respawned_this_frame_.clear();
    return result;
  }


 private:
  std::vector<Resource> resource_map_;
  int amount_of_collected_wood_ = 0;
  int amount_of_collected_stone_= building::kLumberjackNeededStoneAmount.amount_;
  int amount_of_collected_food_ = building::kLumberjackNeededFoodAmount.amount_;

  std::vector<int> harvested_this_frame_;
  std::vector<int> respawned_this_frame_;
};
}
#endif //SAE_ALEXK_CITYBUILDER_API_SRC_RESOURCE_RESOURCEMANAGER_H_
