//
// Created by alexk on 25.06.2026.
//

#include "resource/resource.h"

namespace resource {

Resource::Resource(sf::Vector2i pos, ResourcesType newType)
{
  pos_ = pos;
  type = newType;
  current_state_ = ResourceState::kReady;
}


void Resource::Tick() const{

}
}