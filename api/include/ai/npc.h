//
// Created by alexk on 10.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_NPC_H_
#define SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_NPC_H_


#include <string_view>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "ai/a_start_graph.h"
#include "ai/bt_node.h"
#include "motion/motor.h"
#include "data_utils.h"
#include "resource/resource.h"


namespace api::ai {

enum class WorkingState{
  kPickingUp,
  kFinished,
  kWalking,
};


class Npc {
 public:
  void Setup(const sf::Texture& shared_texture, sf::IntRect texture_rect, sf::Vector2f spawn_position, NpcType type);
  void Update(float dt);
  void Draw(sf::RenderWindow& window);


  void set_path(std::vector<sf::Vector2i> newPath);
  [[nodiscard]] sf::Vector2i const get_current_position(){return { static_cast<int>(motor_.GetPosition().x), static_cast<int>(motor_.GetPosition().y) };}
  [[nodiscard]] sf::Vector2i const get_destination(){return destination_;} ;
  [[nodiscard]] resource::ClosestResource const FindClosestResource(std::span<resource::Resource> resourcesPosition);
  [[nodiscard]] PathStatus const TryToGetPathToClosestResource(std::vector<resource::Resource>& resourcesPosition, AStarGraph& graph, std::span<sf::Vector2i> walkable);
  [[nodiscard]] sf::Vector2i get_house_position() const{ return house_position_;};
  [[nodiscard]] int get_max_steps_astar() const { return kMaxStepsAstar;};
  [[nodiscard]] int TryToFreeResource();


  bool needPath = false;
  PathRequest path_request_ = PathRequest::kNone;


 private:
  // Behaviour-tree actions (bound into the tree via lambdas in Setup()).
  [[nodiscard]] core::ai::behaviour_tree::Status MoveToDestination();
  [[nodiscard]] core::ai::behaviour_tree::Status AskForPath();
  [[nodiscard]] core::ai::behaviour_tree::Status GoBackHome();
  [[nodiscard]] core::ai::behaviour_tree::Status PickUp();

  static constexpr float kSpeed = 200.f;

  std::optional<sf::Sprite> sprite_;


  std::vector<sf::Vector2i> path_;
  sf::Vector2i house_position_;
  sf::Vector2i current_position_;
  sf::Vector2i destination_;
  motion::Motor motor_;
  std::unique_ptr<core::ai::behaviour_tree::Node> bt_root_;
  int current_path_index_;
  NpcType type_;
  std::vector<uint8_t> visited_tiles_;
  std::vector<sf::Vector2i> unreachable_tiles_;

  PathStatus path_status_ = PathStatus::kNone;
  float waited_time_before_asking_for_path_ = 0.f;

  static constexpr int kMaxStepsAstar = 300;
  static constexpr int kMaxStepsFindRsc = 3600;
  static constexpr int kMaxUnreachableRsc = 15;
  static constexpr float kWaitTimeBeforeAskingForPath = 10.f;

  const float kPickingUpTime = 10.f;
  float current_picking_time_ = 0.f;
  WorkingState current_working_state_ = WorkingState::kWalking;
  int current_working_resource_index_ = -1;


  [[nodiscard]] int const CalculateIndexInWorld(int x, int y) const;
  [[nodiscard]] bool const ResourcePositionIsUnreachable(int x, int y) const;
};

constexpr std::array<sf::Vector2i,8> kMoore{
    sf::Vector2i(0,-1), //N
    sf::Vector2i(1,-1), //NE
    sf::Vector2i(1,0),  //E
    sf::Vector2i(1,1),  //SE
    sf::Vector2i(0,1),  //S
    sf::Vector2i(-1,1), //SW
    sf::Vector2i(-1,0), //W
    sf::Vector2i(-1,-1) //NW
};
}  // namespace api::ai

#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_NPC_H_
