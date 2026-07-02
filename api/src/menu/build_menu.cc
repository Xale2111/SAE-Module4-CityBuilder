//
// Created by alexk on 04.06.2026.
//

#include <print>
#include "menu/build_menu.h"
#include "data_utils.h"

namespace menu {

void BuildMenu::Init() {
  if (!build_menu_font_.openFromFile("_assets/fonts/ShareTech-Regular.ttf")) {
    std::println(stderr, "Failed to load build menu font");
  }

  if (!buildings_texture_.loadFromFile("_assets/buildings.png")) {
    std::println(stderr, "Failed to load buildings texture");
  }

  bg_shape_.setFillColor({0, 0, 0, 180});
  bg_shape_.setSize({DataUtils::kScreenWidth * .8f, DataUtils::kScreenHeight / 5});
  position_ = {DataUtils::kScreenWidth * .1f, DataUtils::kScreenHeight / 5 * 4};
  bg_shape_.setPosition(position_);

  toggle_state_ = true;

  SetupToggleButton();
  SetupCardsFont();
  SetupBuildingCards();
  SetupCardsPressEvent();
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
  HandleCardsHover(window);
  HandleCardsInput(event);
}
void BuildMenu::HandleInput(const std::optional<sf::Event> &event) {
  if(!event) return;

  toggle_menu_button_.HandleInput(event);

  if (toggle_state_ && !hovering_menu_) {
    if (const auto *press = event->getIf<sf::Event::MouseButtonPressed>()) {
      if (press->button == sf::Mouse::Button::Left) {
        place_building_press_ = true;
      }
    }
    if (const auto *release = event->getIf<sf::Event::MouseButtonReleased>()) {
      if (release->button == sf::Mouse::Button::Left) {
        if (place_building_press_) {
          try_to_place_building_ = true;
        }
        place_building_press_ = false;
      }
    }
  }

}

void BuildMenu::CheckOverBuildMenu(sf::RenderWindow &window) {
  if (toggle_state_) {
    if (bg_shape_.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
      hovering_menu_ = true;
      return;
    }
  }
  hovering_menu_ = false;
}

void BuildMenu::DrawBuildingHologram(sf::RenderWindow &window, sf::Vector2f snap_position) {
  if (toggle_state_) {
    DisplaySelectedBuilding(window, snap_position);
  }
}

void BuildMenu::ManageHologramColor(bool can_place_building) {
  if (current_display_building_sprite_.has_value()) {
    if (can_place_building) {
      current_display_building_sprite_->setColor(building_can_be_placed_color_);
    } else {
      current_display_building_sprite_->setColor(building_cannot_be_placed_color_);
    }
  }
}

void BuildMenu::ResetMenu() {
  ResetSelectedBuilding();
  toggle_state_ = true;
  hovering_menu_ = false;
  place_building_press_ = false;

  bcui_canteen_.ResetState();
  bcui_food_picker_.ResetState();
  bcui_lumberjack_.ResetState();
  bcui_mine_.ResetState();
  bcui_canteen_.event_on_press_.Clear();
  bcui_food_picker_.event_on_press_.Clear();
  bcui_lumberjack_.event_on_press_.Clear();
  bcui_mine_.event_on_press_.Clear();

  toggle_menu_button_.ResetPressState();
  toggle_menu_button_.event_on_press_.Clear();
}

void BuildMenu::HandleButtonsStates(sf::RenderWindow &window) {
  toggle_menu_button_.CheckHover(sf::Vector2f(sf::Mouse::getPosition(window)));
  toggle_menu_button_.HandleState();
}

void BuildMenu::HandlePanelPosition() {
  if (toggle_state_) {
    position_ = {DataUtils::kScreenWidth * .1f, DataUtils::kScreenHeight / 5 * 4};
  } else {
    position_ = {DataUtils::kScreenWidth * .1f, DataUtils::kScreenHeight};
  }
  toggle_menu_button_.set_position({DataUtils::kScreenWidth / 2, position_.y - kToggleButtonSize.y / 2});
}

void BuildMenu::SetupToggleButton() {
  toggle_menu_button_.set_font(build_menu_font_, 30);
  toggle_menu_button_.set_text("Build Menu");
  toggle_menu_button_.set_size(kToggleButtonSize);
  toggle_menu_button_.set_position({DataUtils::kScreenWidth / 2, position_.y - kToggleButtonSize.y / 2});
  toggle_menu_button_.set_fill_color({89, 82, 61});
  toggle_menu_button_.set_hover_color({61, 56, 41});
  toggle_menu_button_.set_pressed_color({36, 33, 24});
  toggle_menu_button_.ResetPressState();

  toggle_menu_button_.event_on_press_.Append([this]() {
    toggle_state_ = !toggle_state_;
  });
}

void BuildMenu::SetupBuildingCards() {
  //20% width and 90% height
  const sf::Vector2f card_size = {
      bg_shape_.getSize().x * 0.15f,
      bg_shape_.getSize().y * 0.9f
  };

  //space evenly between cards
  const float gap = (bg_shape_.getSize().x - 6.5f * card_size.x) / 5.f;

  //Center Y vertically
  const float card_y = position_.y + bg_shape_.getSize().y * .05f;

  //First card X
  const float first_card_x = position_.x + gap;
  const float card_step = card_size.x + gap;

  // Resources
  std::vector<building::BuildingResource>
      lumberjack_resources = {{{500, ResourcesType::kStone}, {250, ResourcesType::kFood}}};
  std::vector<building::BuildingResource>
      picker_resources = {{{575, ResourcesType::kWood}, {300, ResourcesType::kStone}}};
  std::vector<building::BuildingResource> mine_resources = {{{750, ResourcesType::kWood}}};
  std::vector<building::BuildingResource>
      canteen_resources = {{{1500, ResourcesType::kWood}, {1500, ResourcesType::kStone}, {1500, ResourcesType::kFood}}};

  bcui_food_picker_.Init(card_size,
                         {first_card_x + card_step * 0, card_y},
                         buildings_texture_,
                         0,
                         "Farm",
                         picker_resources);
  bcui_lumberjack_.Init(card_size,
                        {first_card_x + card_step * 1, card_y},
                        buildings_texture_,
                        1,
                        "Lumberjack cabin",
                        lumberjack_resources);
  bcui_mine_.Init(card_size, {first_card_x + card_step * 2, card_y}, buildings_texture_, 2, "Mine", mine_resources);
  bcui_canteen_.Init(card_size,
                     {first_card_x + card_step * 3, card_y},
                     buildings_texture_,
                     3,
                     "Canteen",
                     canteen_resources);
  bcui_canteen_.set_resource_text("1500 : Wood/Stone/Food");
}

void BuildMenu::SetupCardsFont() {
  bcui_lumberjack_.set_font(build_menu_font_, kCardsNameFontSize, kCardsResourceFontSize);
  bcui_food_picker_.set_font(build_menu_font_, kCardsNameFontSize, kCardsResourceFontSize);
  bcui_mine_.set_font(build_menu_font_, kCardsNameFontSize, kCardsResourceFontSize);
  bcui_canteen_.set_font(build_menu_font_, kCardsNameFontSize, kCardsResourceFontSize);
}

void BuildMenu::SetupCardsPressEvent() {
  bcui_lumberjack_.event_on_press_.Append([this]() {
    ChangeSelectedBuildingSprite(bcui_lumberjack_.get_texture_index());
  });
  bcui_food_picker_.event_on_press_.Append([this]() {
    ChangeSelectedBuildingSprite(bcui_food_picker_.get_texture_index());
  });
  bcui_mine_.event_on_press_.Append([this]() {
    ChangeSelectedBuildingSprite(bcui_mine_.get_texture_index());
  });
  bcui_canteen_.event_on_press_.Append([this]() {
    ChangeSelectedBuildingSprite(bcui_canteen_.get_texture_index());
  });
}

void BuildMenu::HandleCardsHover(sf::RenderWindow &window) {
  sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));

  bcui_food_picker_.CheckHover(mousePosition);
  bcui_lumberjack_.CheckHover(mousePosition);
  bcui_mine_.CheckHover(mousePosition);
  bcui_canteen_.CheckHover(mousePosition);
}

void BuildMenu::HandleCardsInput(const std::optional<sf::Event> &event) {
  bcui_food_picker_.HandleInput(event);
  bcui_lumberjack_.HandleInput(event);
  bcui_mine_.HandleInput(event);
  bcui_canteen_.HandleInput(event);
}
void BuildMenu::ChangeSelectedBuildingSprite(int spriteIndex) {
  DisplayableBuilding wanted_building_ = static_cast<DisplayableBuilding>(spriteIndex);
  if (current_selected_building_index_ != wanted_building_) {
    ResetSelectedBuilding();
    current_selected_building_index_ = wanted_building_;
    current_display_building_sprite_ = sf::Sprite(buildings_texture_);
    current_display_building_sprite_->setTextureRect(sf::IntRect{{spriteIndex * 512, 0}, {512, 512}});
    current_display_building_sprite_->setScale({512 / 512 * kCurrentBuildingSpriteSize,
                                                512 / 512 * kCurrentBuildingSpriteSize});
    current_display_building_sprite_->setColor({255, 255, 255, 180});
    current_display_building_sprite_->setOrigin({512 / 2, 512 / 2});
  } else {
    ResetSelectedBuilding();
  }
}

void BuildMenu::DisplaySelectedBuilding(sf::RenderWindow &window, sf::Vector2f snap_position) {
  if (current_display_building_sprite_.has_value()) {
    current_display_building_sprite_->setPosition(snap_position);
    window.draw(*current_display_building_sprite_);
  }
}

void BuildMenu::ResetSelectedBuilding() {
  current_selected_building_index_ = DisplayableBuilding::kNone;
  current_display_building_sprite_.reset();
}
const NpcType BuildMenu::GetNpcTypeBasedOnBuilding() {
  switch (current_selected_building_index_) {
    case DisplayableBuilding::kFoodHouse:return NpcType::kGatherer;
      break;
    case DisplayableBuilding::kLumberjackHouse:return NpcType::kLumberjack;
      break;
    case DisplayableBuilding::kMineHouse:return NpcType::kMiner;
      break;
    case DisplayableBuilding::kCanteen:
    case DisplayableBuilding::kNone:
    default:break;
  }
  return NpcType::kNone;
}

}