//
// Created by alexk on 10.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_SRC_AI_NPC_MANAGER_CC_H_
#define SAE_ALEXK_CITYBUILDER_API_SRC_AI_NPC_MANAGER_CC_H_

#include "ai/npc_manager.h"

namespace api::ai {

void NpcManager::SpawnNpc(std::string_view sprite_path, sf::Vector2f spawn_position) {

    Npc& npc = npcs_.emplace_back();
    npc.Setup(sprite_path, spawn_position);
  /*for (int i = 0; i < 5; ++i) {
  }*/
}

void NpcManager::Update(float dt) {
  for (auto& npc : npcs_) {
    npc.Update(dt);
  }
}

void NpcManager::UpdatePath(std::span<sf::Vector2i> walkables)
{
  for (auto& npc : npcs_) {
    if (npc.needPath)
    {
      npc.set_path(graph_.GetPath(npc.get_current_position(),npc.get_destination(),walkables));
      npc.needPath = false;
    }
  }
}

void NpcManager::Draw(sf::RenderWindow &window) {
  for (auto& npc : npcs_) {
    npc.Draw(window);
  }
}

}
#endif //SAE_ALEXK_CITYBUILDER_API_SRC_AI_NPC_MANAGER_CC_H_
