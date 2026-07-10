//
// Created by alexk on 10.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_SRC_AI_NPC_MANAGER_CC_H_
#define SAE_ALEXK_CITYBUILDER_API_SRC_AI_NPC_MANAGER_CC_H_

#include "ai/npc_manager.h"
#include <print>

namespace api::ai {
NpcManager::NpcManager() {
  if (!npc_textures_.loadFromFile("_assets/pnj.png")) {
    std::println(stderr, "Failed to load npc texture");
  }
  graph_.Setup();
}

void NpcManager::SpawnNpc(NpcType type, sf::Vector2f spawn_position) {

  int source_size = 512;

  sf::IntRect texture_rect(
      {source_size * static_cast<int>(type), 0},
      {source_size, source_size}
  );

  auto npc = std::make_unique<Npc>();
  npc->Setup(npc_textures_, texture_rect, spawn_position, type);
  npcs_.push_back(std::move(npc));
}

void NpcManager::Update(float dt) {
  collected_resources_.clear();
  for (auto &npc : npcs_) {
    npc->Update(dt);

    if (npc->brought_back_resource_home) {
      npc->brought_back_resource_home = false;
      collected_resources_.push_back(npc->get_wanted_resource());
    }
  }
}

void NpcManager::UpdatePath(std::span<sf::Vector2i> walkables, std::vector<resource::Resource>& resourceMap) {
  current_npc_astar_ = 0;
  for (auto &npc : npcs_) {
    //Path management
    if (current_npc_astar_ < max_npc_astar_) {
      current_npc_astar_++;
      if (npc->need_path_) {
        if (npc->path_request_ == PathRequest::kResource) {
          //Could not get a path, canceling the request
          if (npc->TryToGetPathToClosestResource(resourceMap, graph_, walkables) != PathStatus::kNoPath) {
            npc->need_path_ = false;
            npc->path_request_ = PathRequest::kNone;
            continue;
          }
        } else {
          npc->set_path(graph_.GetPath(npc->get_current_position(), npc->get_house_position(), npc->path_request_,
                                       npc->get_max_steps_astar(), walkables));
        }
        npc->need_path_ = false;
        npc->path_request_ = PathRequest::kNone;
      }
    }

    //resource management
    auto resIndex = npc->TryToFreeResource();
    if (resIndex > 0)
    {
      resourceMap[resIndex].NextState();
    }
  }
}

void NpcManager::Draw(sf::RenderWindow &window) {
  for (auto &npc : npcs_) {
    npc->Draw(window);
  }
}

}
#endif //SAE_ALEXK_CITYBUILDER_API_SRC_AI_NPC_MANAGER_CC_H_
