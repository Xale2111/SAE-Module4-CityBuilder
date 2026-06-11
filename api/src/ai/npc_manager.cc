//
// Created by alexk on 10.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_SRC_AI_NPC_MANAGER_CC_H_
#define SAE_ALEXK_CITYBUILDER_API_SRC_AI_NPC_MANAGER_CC_H_

#include "ai/npc_manager.h"

namespace api::ai {
void NpcManager::Setup(std::span<sf::Vector2f> walkable) {

  *walkable_ = std::move(walkable);
}

void NpcManager::SpawnNpc(std::string_view sprite_path, sf::Vector2f spawn_position) {
  npc_.Setup(sprite_path, spawn_position);
}

void NpcManager::Update(float dt) {
  npc_.Update(dt);
}

void NpcManager::Draw(sf::RenderWindow &window) {
  npc_.Draw(window);
}
}
#endif //SAE_ALEXK_CITYBUILDER_API_SRC_AI_NPC_MANAGER_CC_H_
