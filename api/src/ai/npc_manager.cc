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
  walkable_tiles_cache_.resize(DataUtils::kTilemapWidth * DataUtils::kTilemapHeight);
  visited_tiles_cache_.resize(DataUtils::kTilemapWidth * DataUtils::kTilemapHeight);
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
  for (auto &npc : npcs_) {
    npc->Update(dt);
  }
}

void NpcManager::UpdatePath(std::span<sf::Vector2i> walkables, std::span<resource::Resource> resourceMap) {
  for (auto &npc : npcs_) {
    if (npc->needPath) {
      sf::Vector2i destination;
      if (npc->path_request_ == PathRequest::kResource) {
        destination = npc->FindClosestResource(resourceMap);

        // CORRECTION : Si aucune ressource valide n'est trouvée, on annule la requête
        if (destination == sf::Vector2i(-1, -1)) {
          npc->needPath = false;
          npc->path_request_ = PathRequest::kNone;
          continue;
        }
      } else {
        destination = npc->get_house_position();
      }

      // On passe le get_current_position() mis à jour (voir étape 3)
      npc->set_path(graph_.GetPath(npc->get_current_position(), destination, walkables, walkable_tiles_cache_, visited_tiles_cache_));
      npc->needPath = false;
      npc->path_request_ = PathRequest::kNone;
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
