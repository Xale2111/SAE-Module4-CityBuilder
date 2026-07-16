//
// Created by alexk on 25.06.2026.
//

#include "resource/resource.h"
#include <print>

namespace resource {

Resource::Resource(sf::Vector2i pos, ResourcesType newType)
{
  pos_ = pos;
  type = newType;
  current_state_ = ResourceState::kReady;
}


void Resource::Tick(float dt){
  if(current_state_ == ResourceState::kGrowing)
  {
    current_growing_time_ += dt;
    if(IsFullyGrow())
    {
      current_growing_time_ = 0.f;
      NextState();
    }
  }

}
void Resource::NextState() {
  switch (current_state_) {
    case ResourceState::kReady:
      current_state_ = ResourceState::kOccupied;
      break;
    case ResourceState::kOccupied:
      current_state_ = ResourceState::kGrowing;
      just_harvested_ = true;
      break;
    case ResourceState::kGrowing:
      current_state_ = ResourceState::kReady;
      just_respawned_ = true;
      break;
  }
}

bool Resource::IsFullyGrow() {
  bool is_fully_grow = false;
  switch (type) {
    case ResourcesType::kWood:
      is_fully_grow = (current_growing_time_ >= kTreeGrowingTime);
      break;
    case ResourcesType::kStone:
      is_fully_grow = current_growing_time_ >= kStoneGrowingTime;
      break;
    case ResourcesType::kFood:
      is_fully_grow = current_growing_time_ >= kFoodGrowingTime;
      break;
    default: break;
  }

  return is_fully_grow;
}
}