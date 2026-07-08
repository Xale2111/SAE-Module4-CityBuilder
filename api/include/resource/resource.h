//
// Created by alexk on 25.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_INCLUDE_RESOURCE_RESOURCE_H_
#define SAE_ALEXK_CITYBUILDER_API_INCLUDE_RESOURCE_RESOURCE_H_

#include "SFML/System/Vector2.hpp"
#include "data_utils.h"

namespace resource {
struct ClosestResource{
  int index;
  sf::Vector2i position;
};

class Resource {
 public:
  Resource(sf::Vector2i, ResourcesType newType);
  void Tick(float dt);
  [[nodiscard]] sf::Vector2i const get_pos(){return pos_;};
  [[nodiscard]] ResourceState const get_state(){return current_state_;};
  void NextState();

  ResourcesType type;

 private:
  sf::Vector2i pos_;
  ResourceState current_state_;
  float current_growing_time_ = 0.f;
  bool IsFullyGrow();


};
}
#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_RESOURCE_RESOURCE_H_
