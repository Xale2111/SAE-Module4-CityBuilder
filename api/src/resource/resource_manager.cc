//
// Created by alexk on 25.06.2026.
//

#include "resource/resource_manager.h"
#include <print>

namespace resource {

void ResourceManager::AddResource(sf::Vector2i position, ResourcesType type) {
  resource_map_.emplace_back(position, type);
}

void ResourceManager::SetResourceType(int index, ResourcesType newType) {
  resource_map_[index].type = newType;
}

void ResourceManager::Update(float dt) {
  for(auto& resource : resource_map_)
  {
    resource.Tick(dt);
  }
}
void ResourceManager::CollectResources(std::span<ResourcesType> collectedResources) {

  for (auto const& resource : collectedResources) {
    switch (resource) {
      case ResourcesType::kWood:
        amount_of_collected_wood_ += kWoodPerTreeAmount;
        break;
      case ResourcesType::kStone:
        amount_of_collected_stone_ += kStonePerRockAmount;
        break;
      case ResourcesType::kFood:
        amount_of_collected_food_ += kFoodPerFieldAmount;
        break;
      case ResourcesType::kNone:
      default:
        break;
    }
  }

  std::println("Current resources : Wood : {} Stone : {} Food : {}", amount_of_collected_wood_, amount_of_collected_stone_, amount_of_collected_food_);

}

///If any of the needed resource is not available, return false. Else return true.
bool const ResourceManager::CheckAvailableResource(std::span<building::BuildingResource> neededResources) const {
  for (auto const& resource : neededResources) {
    switch (resource.type_) {
      case ResourcesType::kWood:
        if (amount_of_collected_wood_ < resource.amount_) {
          return false;
        }
        break;
      case ResourcesType::kStone:
        if (amount_of_collected_stone_ < resource.amount_) {
          return false;
        }
        break;
        case ResourcesType::kFood:
        if (amount_of_collected_food_ < resource.amount_) {
          return false;
        }
        default: continue;
    }
  }
  return true;
}

///Resource amount should have been checked beforehand.
void ResourceManager::UseResource(std::span<building::BuildingResource> usedResources) {
  for (auto const& resource : usedResources) {
    switch (resource.type_) {
      case ResourcesType::kWood:
        amount_of_collected_wood_ -= resource.amount_;
        break;
      case ResourcesType::kStone:
        amount_of_collected_stone_ -= resource.amount_;
        break;
      case ResourcesType::kFood:
        amount_of_collected_food_ -= resource.amount_;
        break;
        default: continue;
    }
  }
}
void ResourceManager::UseResource(int amount, ResourcesType type) {

  switch (type) {
    case ResourcesType::kWood:
      amount_of_collected_wood_ -= amount;
      break;
    case ResourcesType::kStone:
      amount_of_collected_stone_ -= amount;
      break;
    case ResourcesType::kFood:
      amount_of_collected_food_ -= amount;
      break;
    default: break;
  }
}

}