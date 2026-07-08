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
  for(auto& resource : resource_map_)
  {
    resource.Tick(dt);
  }
}

}