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
  SetupResourceInfosPanel();
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

  //RESOURCES INFOS PANEL
  window.draw(resources_infos_panel_);
  if (panel_title_text_.has_value()) {
    window.draw(*panel_title_text_);
  }

  //wood
  if (wood_info_label_text_.has_value()) {
    window.draw(*wood_info_label_text_);
  }
  if (wood_info_value_text_.has_value()) {
    window.draw(*wood_info_value_text_);
  }

  //stone
  if (stone_info_label_text_.has_value()) {
    window.draw(*stone_info_label_text_);
  }
  if (stone_info_value_text_.has_value()) {
    window.draw(*stone_info_value_text_);
  }

  //food
  if (food_info_label_text_.has_value()) {
    window.draw(*food_info_label_text_);
  }
  if (food_info_value_text_.has_value()) {
    window.draw(*food_info_value_text_);
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


void BuildMenu::UpdateWoodValue(int woodAmount) {
  if(wood_info_value_text_.has_value())
  {
    wood_info_value_text_->setString(std::to_string(woodAmount));
    wood_info_value_text_->setOrigin({wood_info_value_text_->getLocalBounds().size.x / 2, 0});
  }
}

void BuildMenu::UpdateStoneValue(int stoneAmount) {
  if(stone_info_value_text_.has_value())
  {
    stone_info_value_text_->setString(std::to_string(stoneAmount));
    stone_info_value_text_->setOrigin({stone_info_value_text_->getLocalBounds().size.x / 2, 0});
  }
}

void BuildMenu::UpdateFoodValue(int foodAmount) {
  if(food_info_value_text_.has_value())
  {
    food_info_value_text_->setString(std::to_string(foodAmount));
    food_info_value_text_->setOrigin({food_info_value_text_->getLocalBounds().size.x / 2, 0});
  }
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
      lumberjack_resources = {{building::kLumberjackNeededStoneAmount, building::kLumberjackNeededFoodAmount}};
  std::vector<building::BuildingResource> picker_resources = {{building::kPickerNeededWoodAmount,building::kPickerNeededStoneAmount }};
  std::vector<building::BuildingResource> mine_resources = {{building::kMinorNeededWoodAmount}};
  std::vector<building::BuildingResource> canteen_resources = {{building::kCanteenNeededWoodAmount,building::kCanteenNeededStoneAmount,building::kCanteenNeededFoodAmount}};

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
  bcui_canteen_.set_resource_text("3000 : Wood/Stone/Food");
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
    ChangeNeededResourceBasedOnSelectedBuilding(bcui_lumberjack_.get_needed_resources());
  });
  bcui_food_picker_.event_on_press_.Append([this]() {
    ChangeSelectedBuildingSprite(bcui_food_picker_.get_texture_index());
    ChangeNeededResourceBasedOnSelectedBuilding(bcui_food_picker_.get_needed_resources());

  });
  bcui_mine_.event_on_press_.Append([this]() {
    ChangeSelectedBuildingSprite(bcui_mine_.get_texture_index());
    ChangeNeededResourceBasedOnSelectedBuilding(bcui_mine_.get_needed_resources());
  });
  bcui_canteen_.event_on_press_.Append([this]() {
    ChangeSelectedBuildingSprite(bcui_canteen_.get_texture_index());
    ChangeNeededResourceBasedOnSelectedBuilding(bcui_canteen_.get_needed_resources());

  });
}

void BuildMenu::SetupResourceInfosPanel() {
  resources_infos_panel_.setSize({DataUtils::kScreenWidth * .25f, DataUtils::kScreenHeight * .2f});
  resources_infos_panel_.setFillColor({0, 0, 0, 180});
  resources_infos_panel_.setPosition({0,0});

  //setting up the font
  panel_title_text_ = sf::Text(build_menu_font_);
  wood_info_label_text_ = sf::Text(build_menu_font_);
  stone_info_label_text_ = sf::Text(build_menu_font_);
  food_info_label_text_ = sf::Text(build_menu_font_);

  wood_info_value_text_ = sf::Text(build_menu_font_);
  stone_info_value_text_ = sf::Text(build_menu_font_);
  food_info_value_text_ = sf::Text(build_menu_font_);

  //PANEL TITLE
  panel_title_text_->setString("Inventory");
  panel_title_text_->setFillColor({255, 255, 255});
  panel_title_text_->setCharacterSize(48);
  panel_title_text_->setOrigin({panel_title_text_->getLocalBounds().size.x / 2, 0});
  panel_title_text_->setPosition({resources_infos_panel_.getSize().x / 2,10});

  //WOOD INFOS
  wood_info_label_text_->setString("Wood");
  wood_info_label_text_->setFillColor({255, 255, 255});
  wood_info_label_text_->setCharacterSize(32);
  wood_info_label_text_->setOrigin({wood_info_label_text_->getLocalBounds().size.x / 2, 0});
  wood_info_label_text_->setPosition({resources_infos_panel_.getSize().x / 5, 100});

  wood_info_value_text_->setString("0");
  wood_info_value_text_->setFillColor({255, 255, 255});
  wood_info_value_text_->setCharacterSize(24);
  wood_info_value_text_->setOrigin({wood_info_value_text_->getLocalBounds().size.x / 2, 0});
  wood_info_value_text_->setPosition({resources_infos_panel_.getSize().x / 5, 150});

  //STONE INFOS
  stone_info_label_text_->setString("Stone");
  stone_info_label_text_->setFillColor({255, 255, 255});
  stone_info_label_text_->setCharacterSize(32);
  stone_info_label_text_->setOrigin({stone_info_label_text_->getLocalBounds().size.x / 2, 0});
  stone_info_label_text_->setPosition({resources_infos_panel_.getSize().x / 2, 100});

  stone_info_value_text_->setString("0");
  stone_info_value_text_->setFillColor({255, 255, 255});
  stone_info_value_text_->setCharacterSize(24);
  stone_info_value_text_->setOrigin({stone_info_value_text_->getLocalBounds().size.x / 2, 0});
  stone_info_value_text_->setPosition({resources_infos_panel_.getSize().x / 2, 150});

  //FOOD INFOS
  food_info_label_text_->setString("Food");
  food_info_label_text_->setFillColor({255, 255, 255});
  food_info_label_text_->setCharacterSize(32);
  food_info_label_text_->setOrigin({food_info_label_text_->getLocalBounds().size.x / 2, 0});
  food_info_label_text_->setPosition({resources_infos_panel_.getSize().x * 4 / 5, 100});

  food_info_value_text_->setString("0");
  food_info_value_text_->setFillColor({255, 255, 255});
  food_info_value_text_->setCharacterSize(24);
  food_info_value_text_->setOrigin({food_info_value_text_->getLocalBounds().size.x / 2, 0});
  food_info_value_text_->setPosition({resources_infos_panel_.getSize().x *4/5, 150});

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
  current_needed_resources_.clear();
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
void BuildMenu::ChangeNeededResourceBasedOnSelectedBuilding(std::span<building::BuildingResource> neededResources) {
  for (auto const& resource : neededResources) {
    current_needed_resources_.push_back(resource);
  }
/*
  std::string print_needed_resources = "Needed resources: ";
  for (auto const& resource : current_needed_resources_) {
    print_needed_resources += std::to_string(resource.amount_)+ " ";

    switch (resource.type_) {
      case ResourcesType::kWood: print_needed_resources += "Wood";
        break;
      case ResourcesType::kStone: print_needed_resources += "Stone";
        break;
      case ResourcesType::kFood: print_needed_resources += "Food";
        break;
    }
    print_needed_resources += " ";
  }

  std::println("{}", print_needed_resources);*/
}

}