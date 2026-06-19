#include "ai/npc.h"
#include "ai/bt_action.h"
#include "ai/bt_sequence.h"
#include "ai/bt_selector.h"
#include "ai/bt_node_factory.h"

#include <print>

#include <format>
#include "data_utils.h"

namespace api::ai {

// Dans Npc.h
void Npc::Setup(const sf::Texture& shared_texture, sf::IntRect texture_rect, sf::Vector2f spawn_position) {

  sprite_.emplace(shared_texture);
  sprite_->setTextureRect(texture_rect);

  sprite_->setOrigin({texture_rect.size.x / 2.f, texture_rect.size.y / 2.f});

  float scale_x = static_cast<float>(DataUtils::kTileSize / 2) / static_cast<float>(texture_rect.size.x);
  float scale_y = static_cast<float>(DataUtils::kTileSize / 2) / static_cast<float>(texture_rect.size.y);
  sprite_->setScale({scale_x, scale_y});



  srand(time(NULL));

  motor_.SetPosition(spawn_position);
  motor_.SetDestination(spawn_position);  // stay put until the first pick
  motor_.SetSpeed(kSpeed);


  current_position_ = {static_cast<int>(spawn_position.x),static_cast<int>(spawn_position.y)};
  destination_ = {static_cast<int>(spawn_position.x),static_cast<int>(spawn_position.y)};

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
    sprite_->setPosition({motor_.GetPosition().x + DataUtils::kTileSize / 2,motor_.GetPosition().y +DataUtils::kTileSize / 2});
    //sprite_->setPosition(motor_.GetPosition());
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
  int rdm_x = (rand() % DataUtils::kTilemapWidth)*DataUtils::kTileSize;
  int rdm_y = (rand() % DataUtils::kTilemapHeight)*DataUtils::kTileSize;
  std::println("Picking random destination : {};{}" , rdm_x, rdm_y);

  ChangeDestination({rdm_x,rdm_y});

  return core::ai::behaviour_tree::Status::kSuccess;
}

core::ai::behaviour_tree::Status Npc::MoveToDestination() {

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
