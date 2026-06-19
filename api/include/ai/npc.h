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

#include "ai/a_start_graph.h"
#include "ai/bt_node.h"
#include "motion/motor.h"
#include <mdspan>

namespace api::ai {


class Npc {
 public:
  void Setup(const sf::Texture& shared_texture, sf::IntRect texture_rect, sf::Vector2f spawn_position);
  void SetDestination(sf::Vector2f destination);
  void Update(float dt);
//  void AstarPath(std::mdspan<int, std::extents<size_t,2>> tilemap, sf::Vector2f destination);
  void Draw(sf::RenderWindow& window);
  [[nodiscard]] core::ai::behaviour_tree::Status PickRandomDestination();

  void set_path(std::vector<sf::Vector2i> newPath);
  [[nodiscard]] sf::Vector2i const get_current_position(){return current_position_;} ;
  [[nodiscard]] sf::Vector2i const get_destination(){return destination_;} ;

  bool needPath = false;


 private:
  // Behaviour-tree actions (bound into the tree via lambdas in Setup()).
  [[nodiscard]] core::ai::behaviour_tree::Status MoveToDestination();
  void ChangeDestination(sf::Vector2i newDestination);

  static constexpr float kSpeed = 200.f;

  std::optional<sf::Sprite> sprite_;


  std::vector<sf::Vector2i> path_;
  sf::Vector2i current_position_;
  sf::Vector2i destination_;
  motion::Motor motor_;
  std::unique_ptr<core::ai::behaviour_tree::Node> bt_root_;

};

}  // namespace api::ai

#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_NPC_H_
