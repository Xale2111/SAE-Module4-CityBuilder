#include "ai/npc.h"
#include "ai/bt_sequence.h"
#include "ai/bt_node_factory.h"
#include <algorithm>
#include <print>
#include "utils/utils.h"

namespace api::ai {

// Dans Npc.h
void Npc::Setup(const sf::Texture &shared_texture,
                sf::IntRect texture_rect,
                sf::Vector2f spawn_position,
                NpcType type) {

  type_ = type;

  sprite_.emplace(shared_texture);
  sprite_->setTextureRect(texture_rect);

  sprite_->setOrigin({texture_rect.size.x / 2.f, texture_rect.size.y / 2.f});

  float scale_x = static_cast<float>(DataUtils::kTileSize / 2) / static_cast<float>(texture_rect.size.x);
  float scale_y = static_cast<float>(DataUtils::kTileSize / 2) / static_cast<float>(texture_rect.size.y);
  sprite_->setScale({scale_x, scale_y});

  srand(time(NULL));

  motor_.SetPosition(spawn_position);
  motor_.SetDestination(spawn_position);
  motor_.SetSpeed(kSpeed);

  current_position_ = {static_cast<int>(spawn_position.x), static_cast<int>(spawn_position.y)};
  destination_ = {static_cast<int>(spawn_position.x), static_cast<int>(spawn_position.y)};

  house_position_ = {static_cast<int>(spawn_position.x), static_cast<int>(spawn_position.y)};

  using namespace core::ai::behaviour_tree;

  /*
   * simplify :
   * 1. Ask for path
   * 2. Move to closest resource
   * 3. Ask for path to house
   * 4. Move to house
   * 5. if hungry : ask for path to closest possible canteen
   *
   *
   * Final sequence would be :
   * 1. Ask for a path to the destination (given by NpcManager)
   * 2. If npc has a path, move to it and display the npc
   * 3. Once at the resource, ask for path to the house,
   *
   * */

  std::unique_ptr<SequenceNode> wanderSequence = MakeSequence();
  //wanderSequence->AddChild(MakeAction([this] { return PickRandomDestination(); }));
  wanderSequence->AddChild(MakeAction([this] { return AskForPath(); }));
  wanderSequence->AddChild(MakeAction([this] { return MoveToDestination(); }));
  //TODO : Add pick up node (wait at position (oscillate L to R ?) for x seconds, set resource status to growing)
  wanderSequence->AddChild(MakeAction([this] { return GoBackHome(); }));
  wanderSequence->AddChild(MakeAction([this] { return MoveToDestination(); }));

  bt_root_ = std::move(wanderSequence);

  visited_tiles_.resize(DataUtils::kTilemapWidth * DataUtils::kTilemapHeight, false);

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
  if(path_status_ == PathStatus::kNoPath)
  {
    waited_time_before_asking_for_path_ += dt;
    if (waited_time_before_asking_for_path_ >= kWaitTimeBeforeAskingForPath) {
      path_status_ = PathStatus::kNone;
      waited_time_before_asking_for_path_ = 0.f;
      unreachable_tiles_.clear();
      std::println("Reset unreachable tiles");
    }
  }
}

void Npc::Draw(sf::RenderWindow &window) {
  //TODO : Draw npc with vertex instead of sprite
  if (sprite_.has_value()) {
    sprite_->setPosition({motor_.GetPosition().x + DataUtils::kTileSize / 2,
                          motor_.GetPosition().y + DataUtils::kTileSize / 2});
    //sprite_->setPosition(motor_.GetPosition());
    window.draw(*sprite_);
  }
}

void Npc::set_path(std::vector<sf::Vector2i> newPath) {
  path_ = std::move(newPath);

  current_path_index_ = 0;

  if (!path_.empty()) {
    motor_.SetDestination({static_cast<float>(path_[current_path_index_].x),
                           static_cast<float>(path_[current_path_index_].y)});
  } else {
    motor_.SetDestination(motor_.GetPosition());
  }
}

sf::Vector2i const Npc::FindClosestResource(std::span<resource::Resource> resourcesPosition) {

  sf::Vector2i npcPos = {
      static_cast<int>(motor_.GetPosition().x) / DataUtils::kTileSize,
      static_cast<int>(motor_.GetPosition().y) / DataUtils::kTileSize
  };


  if (npcPos.x < 0 || npcPos.x >= DataUtils::kTilemapWidth || npcPos.y < 0 || npcPos.y >= DataUtils::kTilemapHeight) {
    return {-1, -1};
  }

  std::fill(visited_tiles_.begin(), visited_tiles_.end(), false);

  std::vector<sf::Vector2i> open_queue;

  visited_tiles_[CalculateIndexInWorld(npcPos.x, npcPos.y)] = true;
  open_queue.push_back(npcPos);

  ResourcesType wantedResource = ResourcesType::kNone;
  switch (type_) {
    case NpcType::kLumberjack: wantedResource = ResourcesType::kWood; break;
    case NpcType::kMiner:      wantedResource = ResourcesType::kStone; break;
    case NpcType::kGatherer:   wantedResource = ResourcesType::kFood; break;
    default: break;
  }

  int current_index = 0;

  while (current_index < static_cast<int>(open_queue.size()) && current_index < kMaxStepsFindRsc) {
    auto current_pos = open_queue[current_index];
    current_index++;

    int current_idx_world = CalculateIndexInWorld(current_pos.x, current_pos.y);
    auto resource = resourcesPosition[current_idx_world];
    if (resource.type == wantedResource && resource.get_state() == ResourceState::kReady) {
      //TODO: AStar to check if path is possible, if yes, set path, else, add to unreachable tiles and retry next frame
      return {current_pos.x * DataUtils::kTileSize, current_pos.y * DataUtils::kTileSize};
    }

    for (auto neighbour : kMoore) {
      sf::Vector2i new_position = current_pos + neighbour;

      if (new_position.x < 0 || new_position.x >= DataUtils::kTilemapWidth ||
          new_position.y < 0 || new_position.y >= DataUtils::kTilemapHeight) {
        continue;
      }

      int neighbour_idx = CalculateIndexInWorld(new_position.x, new_position.y);

      if (!visited_tiles_[neighbour_idx] && !ResourcePositionIsUnreachable(new_position.x* DataUtils::kTileSize, new_position.y* DataUtils::kTileSize)) {
        visited_tiles_[neighbour_idx] = true;
        open_queue.push_back(new_position);
      }
    }
  }

  return {-1, -1};
}

core::ai::behaviour_tree::Status Npc::MoveToDestination() {

  if (needPath) {
    return core::ai::behaviour_tree::Status::kRunning;
  }

  if (path_.empty()) {
    return core::ai::behaviour_tree::Status::kSuccess;
  }

  if (motor_.RemainingDistance() <= 0.01f) {

    current_path_index_++;

    if (current_path_index_ < path_.size()) {
      motor_.SetDestination({static_cast<float>(path_[current_path_index_].x),
                             static_cast<float>(path_[current_path_index_].y)});
      return core::ai::behaviour_tree::Status::kRunning;
    } else {
      return core::ai::behaviour_tree::Status::kSuccess;
    }
  }

  return core::ai::behaviour_tree::Status::kRunning;
}

core::ai::behaviour_tree::Status Npc::AskForPath() {
  if (path_status_ != PathStatus::kNoPath) {
    needPath = true;
    path_request_ = PathRequest::kResource;
  }
  //TODO : Set resource status to occupied
  return core::ai::behaviour_tree::Status::kSuccess;
}
/*
core::ai::behaviour_tree::Status Npc::PickRandomDestination() {
  int rdm_x = (rand() % DataUtils::kTilemapWidth) * DataUtils::kTileSize;
  int rdm_y = (rand() % DataUtils::kTilemapHeight) * DataUtils::kTileSize;
  std::println("Picking random destination : {};{}", rdm_x, rdm_y);

  ChangeDestination({rdm_x, rdm_y});

  return core::ai::behaviour_tree::Status::kSuccess;
}*/

core::ai::behaviour_tree::Status Npc::GoBackHome() {
  needPath = true;
  path_request_ = PathRequest::kHome;
  return core::ai::behaviour_tree::Status::kSuccess;
}
int const Npc::CalculateIndexInWorld(int x, int y) const {
  return y * DataUtils::kTilemapWidth + x;
}

PathStatus const Npc::TryToGetPathToClosestResource(std::span<resource::Resource> resourcesPosition,AStarGraph& graph, std::span<sf::Vector2i> walkable, std::vector<uint8_t>& cache_walkables, std::vector<uint8_t>& cache_visited) {
  auto destination = FindClosestResource(resourcesPosition);
  path_status_ = PathStatus::kSearching;
  if (destination.x == -1 && destination.y == -1) {
    path_status_ = PathStatus::kNoPath;
    return path_status_;
  }
  auto path = graph.GetPath(current_position_, destination, path_request_, kMaxStepsAstar, walkable, cache_walkables, cache_visited);

  if (!path.empty()) {
    set_path(path);
    path_status_ = PathStatus::kFound;
  }
  else{
    unreachable_tiles_.push_back({destination});
    if (unreachable_tiles_.size() >= kMaxUnreachableRsc) {
      std::println("Too many unreachable tiles, canceling path request. Retrying in {} seconds", kWaitTimeBeforeAskingForPath);;
      path_status_ = PathStatus::kNoPath;
    }
  }

  return path_status_;
}

//acceptable to have a for loop since the size is 5 maximum
bool const Npc::ResourcePositionIsUnreachable(int x, int y) const {
  for (auto unreachable_tile : unreachable_tiles_) {
    if (unreachable_tile.x == x && unreachable_tile.y == y) {
      return true;
    }
  }
  return false;
}

}  // namespace api::ai
