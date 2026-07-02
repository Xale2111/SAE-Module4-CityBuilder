//
// Created by alexk on 10.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_NPC_MANAGER_H_
#define SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_NPC_MANAGER_H_

#include <SFML/System/Vector2.hpp>
#include "graphics/tilemap_renderer.h"
#include <vector>
#include <span>
#include "npc.h"
#include "data_utils.h"


namespace api::ai {

class NpcManager {
 public:
  NpcManager();
  void SpawnNpc(NpcType type, sf::Vector2f spawn_position);
  void Update(float dt);
  void UpdatePath(std::span<sf::Vector2i> walkables,std::span<resource::Resource> resourceMap);
  void Draw(sf::RenderWindow& window);

 private:
  sf::Texture npc_textures_;
  std::vector<std::unique_ptr<Npc>> npcs_;
  std::vector<uint8_t> walkable_tiles_cache_;
  std::vector<uint8_t> visited_tiles_cache_;

  AStarGraph graph_;
  std::vector<sf::Vector2f>  open_list_;
  std::vector<sf::Vector2f> visited_list_;
  std::vector<sf::Vector2f> closed_list_;
};
}
#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_NPC_MANAGER_H_
