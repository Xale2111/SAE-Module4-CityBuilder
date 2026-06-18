#include "ai/npc.h"
#include "ai/bt_action.h"
#include "ai/bt_sequence.h"
#include "ai/bt_selector.h"
#include "ai/bt_node_factory.h"

#include <print>

#include <format>
#include "data_utils.h"

namespace api::ai {

void Npc::Setup(std::string_view sprite_path, const sf::Vector2f start_position) {

  if (texture_->loadFromFile(std::string(sprite_path))) {
    sprite_ = sf::Sprite(*texture_);
  }
  srand(time(NULL));

  motor_.SetPosition(start_position);
  motor_.SetDestination(start_position);  // stay put until the first pick
  motor_.SetSpeed(kSpeed);

  current_position_ = {static_cast<int>(start_position.x),static_cast<int>(start_position.y)};
  destination_ = {static_cast<int>(start_position.x),static_cast<int>(start_position.y)};

  using namespace core::ai::behaviour_tree;


  std::unique_ptr<SequenceNode> wanderSequence = MakeSequence();
  wanderSequence->AddChild(MakeAction([this] { return PickRandomDestination(); }));
  wanderSequence->AddChild(MakeAction([this] { return MoveToDestination(); }));


  bt_root_ = std::move(wanderSequence);

  // Rough wander behaviour:
  // Sequence (pick a random destination, then move to it)
  // PickRandomDestination always succeeds, MoveToDestination stays kRunning
  // until the motor reaches the target. When the sequence completes, it resets

}

void Npc::Update(const float dt) {
  motor_.Update(dt);
  if (bt_root_) {
    bt_root_->Tick();
  }
}

void Npc::Draw(sf::RenderWindow &window) {
  if (sprite_.has_value()) {
    sprite_->setPosition(motor_.GetPosition());
    window.draw(*sprite_);
  }
}

void Npc::set_path(std::vector<sf::Vector2i> newPath){
  path_ = std::move(newPath);

  if(!path_.empty())
  {
    motor_.SetDestination({static_cast<float>(path_.front().x),static_cast<float>(path_.front().y)});
  }
  else
  {
    motor_.SetDestination(motor_.GetPosition());
  }
}

core::ai::behaviour_tree::Status Npc::PickRandomDestination() {
  std::println("Picking random destination");
  int rdm_x = (rand() % DataUtils::kTilemapWidth)*DataUtils::kTileSize;
  int rdm_y = (rand() % DataUtils::kTilemapHeight)*DataUtils::kTileSize;

  ChangeDestination({rdm_x,rdm_y});

  return core::ai::behaviour_tree::Status::kSuccess;
}

core::ai::behaviour_tree::Status Npc::MoveToDestination() {
  std::println("Moving to destination");
  if (needPath) {
    return core::ai::behaviour_tree::Status::kRunning;
  }

  if(motor_.RemainingDistance() <= 0.01f) {

    if (!path_.empty()) {
      path_.erase(path_.begin());
    }

    if (!path_.empty()) {
      motor_.SetDestination({static_cast<float>(path_.front().x), static_cast<float>(path_.front().y)});
      return core::ai::behaviour_tree::Status::kRunning;
    } else {
      return core::ai::behaviour_tree::Status::kSuccess;
    }
  }

  return core::ai::behaviour_tree::Status::kRunning;
}
void Npc::ChangeDestination(sf::Vector2i newDestination) {
  current_position_ = {static_cast<int>(motor_.GetPosition().x),static_cast<int>(motor_.GetPosition().y)};
  destination_ = newDestination;
  needPath = true;

}

}  // namespace api::ai
