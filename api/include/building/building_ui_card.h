//
// Created by alexk on 04.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_SRC_BUILDING_BUILDING_UI_CARD_H_
#define SAE_ALEXK_CITYBUILDER_API_SRC_BUILDING_BUILDING_UI_CARD_H_

#include "SFML/Graphics.hpp"
#include "building_resource.h"
#include <span>
#include "event.h"

namespace building {
constexpr std::string_view kBuildingsSpritePath = "_assets/buildings.png";

class BuildingCardUI {
 public:
  CustomEvent event_on_press_;

  void Init(sf::Vector2f size,
            sf::Vector2f position,
            const sf::Texture& texture,
            int texture_index,
            std::string_view name,
            std::vector<BuildingResource>);
  [[nodiscard]] std::span<BuildingResource> get_needed_resources() { return needed_resources_; };
  [[nodiscard]] const int get_texture_index(){return texture_index_;};

  void ResetState();

  void Draw(sf::RenderWindow &window);
  void CheckHover(sf::Vector2f mousePosition);
  void HandleInput(const std::optional<sf::Event>& event);
  void set_font(sf::Font& font, int name_size, int resources_size);
  void set_resource_text(std::string_view new_text);

 private:
  sf::RectangleShape bg_card_;
  sf::RectangleShape bg_name_;
  sf::RectangleShape bg_sprite_;
  sf::RectangleShape bg_resources_;
  std::string_view building_name_;
  std::optional<sf::Text> name_text_;
  std::optional<sf::Text> resources_text_;
  std::optional<sf::Sprite> building_sprite_;
  int texture_index_;

  bool is_mouse_hover_ = false;
  bool is_pressed_ = false;


  sf::FloatRect image_rect_;
  std::vector<BuildingResource> needed_resources_;

  const sf::Color default_color_{255, 255, 255, 100};
  const sf::Color hover_color_{255, 255, 255, 160};

  void set_backgrounds_color(sf::Color newColor);

  void SetupBackground(sf::RectangleShape &background,
                       sf::Vector2f cardPosition,
                       sf::Vector2f cardSize,
                       sf::Vector2f bgSizePercent,
                       sf::Vector2f bgPositionPercent);

};
}

#endif //SAE_ALEXK_CITYBUILDER_API_SRC_BUILDING_BUILDING_UI_CARD_H_
