//
// Created by alexk on 04.06.2026.
//

#include <print>
#include "building/building_ui_card.h"

namespace building {

void BuildingCardUI::Init(sf::Vector2f size,
                          sf::Vector2f position,
                          const sf::Texture &texture,
                          int texture_index,
                          std::string_view name,
                          std::vector<BuildingResource> needed_resources) {

  bg_card_.setSize(size);
  bg_card_.setPosition(position);
  bg_card_.setFillColor(default_color_);
  bg_card_.setOutlineThickness(2);
  bg_card_.setOutlineColor(sf::Color::White);

  texture_index_ = texture_index;

  SetupBackground(bg_name_, position, size, {90.f, 20.f}, {5.f, 3.f});
  SetupBackground(bg_sprite_, position, size, {90.f, 40.f}, {5.f, 28.f});
  SetupBackground(bg_resources_, position, size, {90.f, 20.f}, {5.f, 73.f});

  building_name_ = name;
  needed_resources_ = std::move(needed_resources);

  if (name_text_.has_value()) {
    name_text_->setString(building_name_.data());
    name_text_->setPosition({bg_name_.getPosition().x,
                             bg_name_.getPosition().y + bg_name_.getSize().y / 4.f});
  }

  if (resources_text_.has_value()) {
    std::string resources_string;
    for (const auto &resource : needed_resources_) {
      resources_string += std::to_string(resource.amount_) + " : ";
      switch (resource.type_) {
        case ResourcesType::kWood: resources_string += "Wood";
          break;
        case ResourcesType::kStone: resources_string += "Stone";
          break;
        case ResourcesType::kFood: resources_string += "Food";
          break;
      }
      resources_string += "   ";
    }
    resources_text_->setString(resources_string);
    resources_text_->setPosition({bg_resources_.getPosition().x,
                                  bg_resources_.getPosition().y + bg_resources_.getSize().y / 4.f});
  }
  name_text_->setPosition({bg_name_.getPosition().x, bg_name_.getPosition().y + bg_name_.getSize().y / 4});
  resources_text_->setPosition({bg_resources_.getPosition().x,
                                bg_resources_.getPosition().y + bg_resources_.getSize().y / 4});

  // Setup sprite
  building_sprite_ = sf::Sprite(texture);
  const float sprite_size = bg_sprite_.getSize().y * .9f;
  building_sprite_->setTextureRect(sf::IntRect{{texture_index * 512, 0}, {512, 512}});
  building_sprite_->setScale({sprite_size / 512, sprite_size / 512});

  float halfSprite = building_sprite_->getGlobalBounds().size.x / 2;

  building_sprite_->setPosition({bg_sprite_.getPosition().x + bg_sprite_.getSize().x / 2 - halfSprite,
                                 bg_sprite_.getPosition().y + bg_sprite_.getSize().y * .05f});
}

void BuildingCardUI::Draw(sf::RenderWindow &window) {
  window.draw(bg_card_);
  window.draw(bg_name_);
  window.draw(bg_sprite_);
  window.draw(bg_resources_);
  if (building_sprite_.has_value()) {
    window.draw(*building_sprite_);
  }
  window.draw(name_text_.value());
  window.draw(resources_text_.value());
}

void BuildingCardUI::CheckHover(sf::Vector2f mousePosition) {
  if (bg_card_.getGlobalBounds().contains(mousePosition)) {
    set_backgrounds_color(hover_color_);
    is_mouse_hover_ = true;
  } else {
    set_backgrounds_color(default_color_);
    is_mouse_hover_ = false;
  }
}

void BuildingCardUI::HandleInput(const std::optional<sf::Event> &event) {
  if (is_mouse_hover_) {
    if (const auto *mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
      is_pressed_ = true;
    }
    if (const auto *mouse = event->getIf<sf::Event::MouseButtonReleased>()) {
      if (is_pressed_) {
        event_on_press_.Invoke();
      }
      is_pressed_ = false;
    }
  } else {
    is_pressed_ = false;
  }
}

void BuildingCardUI::set_font(sf::Font &font, int name_size, int resources_size) {
  name_text_ = sf::Text(font);
  name_text_->setCharacterSize(name_size);
  resources_text_ = sf::Text(font);
  resources_text_->setCharacterSize(resources_size);
}

void BuildingCardUI::set_resource_text(std::string_view new_text) {
  if (resources_text_.has_value()) {
    resources_text_->setString(new_text.data());
  }
}
void BuildingCardUI::set_backgrounds_color(sf::Color newColor) {
  bg_card_.setFillColor(newColor);
  bg_name_.setFillColor(newColor);
  bg_sprite_.setFillColor(newColor);
  bg_resources_.setFillColor(newColor);
}

void BuildingCardUI::SetupBackground(sf::RectangleShape &background,
                                     sf::Vector2f cardPosition,
                                     sf::Vector2f cardSize,
                                     sf::Vector2f bgSizePercent,
                                     sf::Vector2f bgPositionPercent) {
  float size_x_percent = cardSize.x / 100;  //one percent of the width
  float size_y_percent = cardSize.y / 100;  //one percent of the height

  background.setSize({size_x_percent * bgSizePercent.x, size_y_percent * bgSizePercent.y});
  background.setPosition({cardPosition.x + cardSize.x * bgPositionPercent.x / 100.f,
                          cardPosition.y + cardSize.y * bgPositionPercent.y / 100.f});
  background.setFillColor(default_color_);
  background.setOutlineThickness(2);
  background.setOutlineColor(sf::Color::White);
}

}