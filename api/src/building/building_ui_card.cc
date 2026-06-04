//
// Created by alexk on 04.06.2026.
//

#include <print>
#include "building/building_ui_card.h"

namespace building {

void BuildingCardUI::Init(sf::Vector2f size,
                          sf::Vector2f position,
                          int texture_index,
                          std::string_view name,
                          std::span<BuildingResource> needed_resources) {

  std::print("Card init at position: {}, {}\n", position.x, position.y);
  std::print("Card size: {}, {}\n", size.x, size.y);

  bg_card_.setSize(size);
  bg_card_.setPosition(position);
  bg_card_.setFillColor(sf::Color{255,255,255,100});
  bg_card_.setOutlineThickness(2);
  bg_card_.setOutlineColor(sf::Color::White);

  SetupBackground(bg_name_,position, size, {90.f, 20.f}, {5.f, 3.f});
  SetupBackground(bg_card_,position, size, {90.f, 50.f}, { 5.f, 28.f});
  SetupBackground(bg_resources_, position,size, {90.f, 20.f}, {5.f, 83.f});

  building_name_ = name;
  image_rect_ = {{texture_index * 512.f, 0}, {512, 512}};

  std::print("bg_card_ position: {}, {}\n", bg_card_.getPosition().x, bg_card_.getPosition().y);
  std::print("bg_card_ size: {}, {}\n", bg_card_.getSize().x, bg_card_.getSize().y);
}

void BuildingCardUI::Draw(sf::RenderWindow &window) {
  std::print("Drawing card at: {}, {}\n", bg_card_.getPosition().x, bg_card_.getPosition().y);
  window.draw(bg_card_);
  window.draw(bg_name_);
  window.draw(bg_sprite_);
  window.draw(bg_resources_);
}

void BuildingCardUI::SetupBackground(sf::RectangleShape &background,sf::Vector2f cardPosition, sf::Vector2f cardSize, sf::Vector2f bgSizePercent, sf::Vector2f bgPositionPercent) {
  float size_x_percent = cardSize.x/100;  //one percent of the width
  float size_y_percent = cardSize.y/100;  //one percent of the height

  background.setSize({size_x_percent*bgSizePercent.x, size_y_percent*bgSizePercent.y});
  background.setPosition({cardPosition.x + cardSize.x * bgPositionPercent.x / 100.f,cardPosition.y + cardSize.y * bgPositionPercent.y / 100.f });
  background.setFillColor(sf::Color{255,255,255,100});
  background.setOutlineThickness(2);
  background.setOutlineColor(sf::Color::White);
}

}