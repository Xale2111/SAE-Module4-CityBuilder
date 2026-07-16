//
// Created by alexk on 25.06.2026.
//

#include "resource/resource_manager.h"


namespace resource {

void ResourceManager::AddResource(sf::Vector2i position, ResourcesType type) {
  resource_map_.emplace_back(position, type);
}

void ResourceManager::SetResourceType(int index, ResourcesType newType) {
  resource_map_[index].type = newType;
}

void ResourceManager::Update(float dt) {
  harvested_this_frame_.clear();
  respawned_this_frame_.clear();

  for (int i = 0; i < static_cast<int>(resource_map_.size()); i++) {
    resource_map_[i].Tick(dt);

    if (resource_map_[i].just_harvested_) {
      harvested_this_frame_.push_back(i);
      resource_map_[i].just_harvested_ = false;
    }
    if (resource_map_[i].just_respawned_) {
      respawned_this_frame_.push_back(i);
      resource_map_[i].just_respawned_ = false;
    }
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