//
// Created by alexk on 10.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_NPC_H_
#define SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_NPC_H_


#include <memory>
#include <optional>
#include <random>
#include <string_view>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "ai/bt_node.h"
#include "motion/motor.h"
#include <mdspan>

namespace api::ai {

// A "rough" NPC: a sprite that wanders the map, driven by a behaviour tree.
// No pathfinding -- movement is handled by a simple linear Motor.
class Npc {
 public:
  void Setup(std::string_view sprite_path, sf::Vector2f start_position);
  void SetDestination(sf::Vector2f destination);
  void Update(float dt);
//  void AstarPath(std::mdspan<int, std::extents<size_t,2>> tilemap, sf::Vector2f destination);
  void Draw(sf::RenderWindow& window);

 private:
  // Behaviour-tree actions (bound into the tree via lambdas in Setup()).
  [[nodiscard]] core::ai::behaviour_tree::Status MoveToDestination() const;

  static constexpr float kSpeed = 300.f;

  std::unique_ptr<sf::Texture> texture_ = std::make_unique<sf::Texture>();
  std::optional<sf::Sprite> sprite_;

  motion::Motor motor_;
  std::unique_ptr<core::ai::behaviour_tree::Node> bt_root_;

};

}  // namespace api::ai

#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_NPC_H_
