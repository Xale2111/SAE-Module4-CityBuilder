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

  switch (type_) {
    case NpcType::kLumberjack: wanted_resource_ = ResourcesType::kWood; break;
    case NpcType::kMiner: wanted_resource_ = ResourcesType::kStone; break;
    case NpcType::kGatherer: wanted_resource_ = ResourcesType::kFood; break;
    default: break;
  }


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

  std::unique_ptr<SequenceNode> pickUpSequence = MakeSequence();
  //pickUpSequence->AddChild(MakeAction([this] { return PickRandomDestination(); }));
  pickUpSequence->AddChild(MakeAction([this] { return AskForPath(); }));
  pickUpSequence->AddChild(MakeAction([this] { return MoveToDestination(); }));
  pickUpSequence->AddChild(MakeAction([this] { return PickUp(); }));
  pickUpSequence->AddChild(MakeAction([this] { return GoBackHome(); }));
  pickUpSequence->AddChild(MakeAction([this] { return MoveToDestination(); }));

  bt_root_ = std::move(pickUpSequence);

  visited_tiles_.resize(DataUtils::kTilemapWidth * DataUtils::kTilemapHeight, false);

  // Rough wander behaviour:
  // Sequence (pick a random destination, then move to it)
  // PickRandomDestination always succeeds, MoveToDestination stays kRunning
  // until the motor reaches the target. When the sequence completes, it resets

}

void Npc::Update(const float dt) {
  motor_.Update(dt);
  //If is picking up the resource, update the picking time
  if(current_working_state_ == WorkingState::kPickingUp)
  {
    current_picking_time_ += dt;
  }
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

resource::ClosestResource const Npc::FindClosestResource(std::span<resource::Resource> resourcesPosition) {

  sf::Vector2i npcPos = {
      static_cast<int>(motor_.GetPosition().x) / DataUtils::kTileSize,
      static_cast<int>(motor_.GetPosition().y) / DataUtils::kTileSize
  };


  if (npcPos.x < 0 || npcPos.x >= DataUtils::kTilemapWidth || npcPos.y < 0 || npcPos.y >= DataUtils::kTilemapHeight) {
    return {-1,{-1, -1}}; //Not in map, returning an impossible index and position
  }

  std::fill(visited_tiles_.begin(), visited_tiles_.end(), false);

  std::vector<sf::Vector2i> open_queue;

  visited_tiles_[CalculateIndexInWorld(npcPos.x, npcPos.y)] = true;
  open_queue.push_back(npcPos);

  int current_index = 0;

  while (current_index < static_cast<int>(open_queue.size()) && current_index < kMaxStepsFindRsc) {
    auto current_pos = open_queue[current_index];
    current_index++;

    int current_idx_world = CalculateIndexInWorld(current_pos.x, current_pos.y);
    auto resource = resourcesPosition[current_idx_world];
    if (resource.type == wanted_resource_ && resource.get_state() == ResourceState::kReady) {
      return {current_idx_world,{current_pos.x * DataUtils::kTileSize, current_pos.y * DataUtils::kTileSize}};
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

  return {-1,{-1, -1}};
}

core::ai::behaviour_tree::Status Npc::MoveToDestination() {

  if (need_path_) {
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
    need_path_ = true;
    path_request_ = PathRequest::kResource;
  }
  return core::ai::behaviour_tree::Status::kSuccess;
}

core::ai::behaviour_tree::Status Npc::GoBackHome() {
  need_path_ = true;
  path_request_ = PathRequest::kHome;
  return core::ai::behaviour_tree::Status::kSuccess;
}

core::ai::behaviour_tree::Status Npc::PickUp() {
  current_working_state_ = WorkingState::kPickingUp;
  if (current_picking_time_ >= kPickingUpTime) {
    current_working_state_ = WorkingState::kFinished;
    brought_back_resource_home = true;
    return core::ai::behaviour_tree::Status::kSuccess;
  }
  return core::ai::behaviour_tree::Status::kRunning;
}

int const Npc::CalculateIndexInWorld(int x, int y) const {
  return y * DataUtils::kTilemapWidth + x;
}

PathStatus const Npc::TryToGetPathToClosestResource(std::vector<resource::Resource>& resourcesPosition,AStarGraph& graph, std::span<sf::Vector2i> walkable) {

  auto closest_resource = FindClosestResource(resourcesPosition);
  path_status_ = PathStatus::kSearching;
  if (closest_resource.index < 0) {
    path_status_ = PathStatus::kNoPath;
    return path_status_;
  }
  auto path = graph.GetPath(current_position_, closest_resource.position, path_request_, kMaxStepsAstar, walkable);

  if (!path.empty()) {
    set_path(path);
    //If path is possible, set the resource to occupied, set the current resource index the npc is working on and reset timer
    resourcesPosition[closest_resource.index].NextState();
    current_working_resource_index_ = closest_resource.index;
    current_picking_time_ = 0.f;
    path_status_ = PathStatus::kFound;
  }
  else{
    unreachable_tiles_.push_back({closest_resource.position});
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

int Npc::TryToFreeResource() {
  if(current_working_state_ == WorkingState::kFinished)
  {
    current_working_state_ = WorkingState::kWalking;
    return current_working_resource_index_;
  }
  return -1;
}
}  // namespace api::ai
