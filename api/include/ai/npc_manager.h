//
// Created by alexk on 10.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_NPC_MANAGER_H_
#define SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_NPC_MANAGER_H_

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <span>
#include "npc.h"

namespace api::ai {

class NpcManager {
 public:
  void Setup(std::span<sf::Vector2f> walkable);

  void SpawnNpc(std::string_view sprite_path, sf::Vector2f spawn_position);
  void Update(float dt);
  void Draw(sf::RenderWindow& window);

 private:
  Npc npc_;

  const sf::Vector2f map_size_ = {DataUtils::kTilemapWidth,DataUtils::kTilemapHeight};

  std::unique_ptr<std::span<sf::Vector2f>> walkable_;

  std::vector<sf::Vector2f>  open_list_;
  std::vector<sf::Vector2f> visited_list_;
  std::vector<sf::Vector2f> closed_list_;
};
}
#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_AI_NPC_MANAGER_H_
