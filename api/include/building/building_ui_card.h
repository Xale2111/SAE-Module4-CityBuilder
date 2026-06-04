//
// Created by alexk on 04.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_SRC_BUILDING_BUILDING_UI_CARD_H_
#define SAE_ALEXK_CITYBUILDER_API_SRC_BUILDING_BUILDING_UI_CARD_H_

#include "SFML/Graphics.hpp"
#include "building_resource.h"
#include <span>

namespace building {
constexpr std::string_view kBuildingsSpritePath = "_assets/buildings.png";

class BuildingCardUI {
 public:
  void Init(sf::Vector2f size,
            sf::Vector2f position,
            int texture_index,
            std::string_view name,
            std::span<BuildingResource>);
  [[nodiscard]] std::span<const BuildingResource> get_needed_resources() { return needed_resources_; };

void Draw(sf::RenderWindow& window);

 private:
  sf::RectangleShape bg_card_;
  sf::RectangleShape bg_name_;
  sf::RectangleShape bg_sprite_;
  sf::RectangleShape bg_resources_;
  std::string_view building_name_;
  sf::FloatRect image_rect_;
  std::vector<BuildingResource> needed_resources_;

  void SetupBackground(sf::RectangleShape& background, sf::Vector2f cardPosition,sf::Vector2f cardSize, sf::Vector2f bgSizePercent, sf::Vector2f bgPositionPercent);

};
}

#endif //SAE_ALEXK_CITYBUILDER_API_SRC_BUILDING_BUILDING_UI_CARD_H_
