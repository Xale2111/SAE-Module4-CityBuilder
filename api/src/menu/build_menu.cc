//
// Created by alexk on 04.06.2026.
//

#include <iostream>
#include "menu/build_menu.h"
#include "data_utils.h"

namespace menu {

void BuildMenu::Init() {
  if (!build_menu_font_.openFromFile("_assets/fonts/ShareTech-Regular.ttf")) {
    std::cerr << "Failed to load build menu font";
  }

  bg_shape_.setFillColor({0, 0, 0, 180});
  bg_shape_.setSize({DataUtils::kScreenWidth * .8f, DataUtils::kScreenHeight / 5});
  position_ = {DataUtils::kScreenWidth * .1f, DataUtils::kScreenHeight / 5 * 4};
  bg_shape_.setPosition(position_);

  toggle_state_ = true;

  SetupToggleButton();
  SetupBuildingCards();
}

void BuildMenu::Draw(sf::RenderWindow &window) {
  toggle_menu_button_.Draw(window);
  if (toggle_state_) {
    window.draw(bg_shape_);

    bcui_food_picker_.Draw(window);
    bcui_lumberjack_.Draw(window);
    bcui_mine_.Draw(window);
    bcui_canteen_.Draw(window);
  }
}

void BuildMenu::HandleMenu(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
  HandleButtonsStates(window);
  HandleInput(event);
  HandlePanelPosition();
}

void BuildMenu::HandleButtonsStates(sf::RenderWindow &window) {
  toggle_menu_button_.CheckHover(sf::Vector2f(sf::Mouse::getPosition(window)));
  toggle_menu_button_.HandleState();
}
void BuildMenu::HandleInput(const std::optional<sf::Event> &event) {
  toggle_menu_button_.HandleInput(event);
}
void BuildMenu::HandlePanelPosition() {
  if (toggle_state_) {
    position_ = {DataUtils::kScreenWidth * .1f, DataUtils::kScreenHeight / 4 * 3};
  } else {
    position_ = {DataUtils::kScreenWidth * .1f, DataUtils::kScreenHeight};
  }
  toggle_menu_button_.set_position({DataUtils::kScreenWidth / 2, position_.y - toggle_button_size_.y / 2});

}
void BuildMenu::SetupToggleButton() {
  toggle_menu_button_.set_font(build_menu_font_, 30);
  toggle_menu_button_.set_text("Build Menu");
  toggle_menu_button_.set_size(toggle_button_size_);
  toggle_menu_button_.set_position({DataUtils::kScreenWidth / 2, position_.y - toggle_button_size_.y / 2});
  toggle_menu_button_.set_fill_color({89, 82, 61});
  toggle_menu_button_.set_hover_color({61, 56, 41});
  toggle_menu_button_.set_pressed_color({36, 33, 24});
  toggle_menu_button_.ResetPressState();

  toggle_menu_button_.OnPress += [this]() {
    toggle_state_ = !toggle_state_;
  };
}

void BuildMenu::SetupBuildingCards() {
  // Taille de la carte : 20% largeur et 90% hauteur du bg_shape_
  const sf::Vector2f card_size = {
      bg_shape_.getSize().x * 0.15f,
      bg_shape_.getSize().y * 0.9f
  };

  // 5 espaces égaux pour 4 cartes : | gap | carte | gap | carte | gap | carte | gap | carte | gap |
  const float gap = (bg_shape_.getSize().x - 6 * card_size.x) / 5.f;

  // Y centré verticalement dans le bg (origin est au centre de la carte)
  const float card_y = position_.y + bg_shape_.getSize().y * .05f;

  // X de la première carte
  const float first_card_x = position_.x + gap;
  const float card_step = card_size.x + gap;

  // Resources
  std::array<building::BuildingResource, 2>
      lumberjack_resources = {{{500, ResourcesType::kStone}, {250, ResourcesType::kFood}}};
  std::array<building::BuildingResource, 2>
      picker_resources = {{{575, ResourcesType::kWood}, {300, ResourcesType::kStone}}};
  std::array<building::BuildingResource, 1> mine_resources = {{{750, ResourcesType::kWood}}};

  bcui_food_picker_.Init(card_size, {first_card_x + card_step * 0, card_y}, 0, "Farm", picker_resources);
  bcui_lumberjack_.Init(card_size, {first_card_x + card_step * 1, card_y}, 1, "Lumberjack cabin", lumberjack_resources);
  bcui_mine_.Init(card_size, {first_card_x + card_step * 2, card_y}, 2, "Mine", mine_resources);
  bcui_canteen_.Init(card_size, {first_card_x + card_step * 3, card_y}, 3, "Canteen", {});
}

}

