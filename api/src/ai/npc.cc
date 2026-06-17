#include "ai/npc.h"
#include "ai/bt_action.h"
#include "ai/bt_sequence.h"
#include "ai/bt_selector.h"
#include "ai/bt_node_factory.h"

#include <format>
#include "data_utils.h"

namespace api::ai {

void Npc::Setup(std::string_view sprite_path, const sf::Vector2f start_position, std::vector<sf::Vector2i>& walkable) {

  if (texture_->loadFromFile(std::string(sprite_path))) {
    sprite_ = sf::Sprite(*texture_);
  }

  walkable_ = walkable;
  motor_.SetPosition(start_position);
  motor_.SetDestination(start_position);  // stay put until the first pick
  motor_.SetSpeed(kSpeed);

  using namespace core::ai::behaviour_tree;

  auto alwaysSuccess = MakeAction([] { return Status::kSuccess; });

  std::unique_ptr<SelectorNode> selector = std::make_unique<SelectorNode>();
  selector->AddChild(MakeAction([this] { return Status::kFailure; }));



  std::unique_ptr<SequenceNode> wanderSequence = MakeSequence();
  wanderSequence->AddChild(MakeAction([this] { return PickRandomDestination(); }));
  wanderSequence->AddChild(MakeAction([this] { return MoveToDestination(); }));

  selector->AddChild(std::move(wanderSequence));

  bt_root_ = std::move(selector);

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


void Npc::SetDestination(sf::Vector2f destination) {
  sf::Vector2i start = {static_cast<int>(motor_.GetPosition().x),static_cast<int>(motor_.GetPosition().y)};
  sf::Vector2i end = {static_cast<int>(destination.x),static_cast<int>(destination.y)};

  path_ = graph_.GetPath(start, end, walkable_);

  if (!path_.empty()) {
    motor_.SetDestination({static_cast<float>(path_.front().x * DataUtils::kTileSize),
                           static_cast<float>(path_.front().y * DataUtils::kTileSize)});
  } else {
    // Aucun chemin trouvé, rester sur place ou gérer l'erreur
    motor_.SetDestination(motor_.GetPosition());
  }

}

core::ai::behaviour_tree::Status Npc::PickRandomDestination() {

  srand(time(NULL));
  float rdm_x = (rand() % DataUtils::kTilemapWidth)*DataUtils::kTileSize;
  float rdm_y = (rand() % DataUtils::kTilemapHeight)*DataUtils::kTileSize;

  SetDestination({rdm_x,rdm_y});

  return core::ai::behaviour_tree::Status::kSuccess;
}

core::ai::behaviour_tree::Status Npc::MoveToDestination() const {
  return motor_.RemainingDistance() <= 0.001f ? core::ai::behaviour_tree::Status::kSuccess
                                              : core::ai::behaviour_tree::Status::kRunning;
}

}  // namespace api::ai
