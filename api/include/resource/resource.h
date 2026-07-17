//
// Created by alexk on 25.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_INCLUDE_RESOURCE_RESOURCE_H_
#define SAE_ALEXK_CITYBUILDER_API_INCLUDE_RESOURCE_RESOURCE_H_

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "data_utils.h"
#include <vector>
#include <optional>

namespace resource {
struct ClosestResource{
  int index;
  sf::Vector2i position;
};

class Resource {
 public:
  Resource(sf::Vector2i, ResourcesType newType);
  void Tick(float dt);
  [[nodiscard]] sf::Vector2i get_pos() const { return pos_; }
  [[nodiscard]] ResourceState get_state() const { return current_state_; }
  void set_state(ResourceState state);
  void NextState();

  ResourcesType type;
  bool just_harvested_ = false;  // kOccupied → kGrowing
  bool just_respawned_ = false;  // kGrowing → kReady

  //TODO : Add vertex of the resources (we need to have a ref to the vertices of the resource so we can update it when changing the state)

 private:
  sf::Vector2i pos_;
  ResourceState current_state_;
  float current_growing_time_ = 0.f;
  bool IsFullyGrow() const;
};
}
#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_RESOURCE_RESOURCE_H_
