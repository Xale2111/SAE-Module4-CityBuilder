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

void ResourceManager::Update() {

  //Tick only growing and picked up resources
  //no need to tick ready resources

}

}