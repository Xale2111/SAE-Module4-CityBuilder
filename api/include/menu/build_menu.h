//
// Created by alexk on 04.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_SRC_MENU_BUILD_MENU_H_
#define SAE_ALEXK_CITYBUILDER_API_SRC_MENU_BUILD_MENU_H_

#include "SFML/Graphics.hpp"
#include "button.h"
#include "building/building_ui_card.h"
#include "building/building_resource.h"

namespace menu {

class BuildMenu {
 public:
  void Init();
  void Draw(sf::RenderWindow &window);

  /*
  void SelectBuilding();
  void PlaceBuilding();
  void RemoveBuilding();
  void MoveBuilding();*/

  void HandleMenu(sf::RenderWindow &window, const std::optional<sf::Event> &event);
  void HandleInput(const std::optional<sf::Event> &event);
  void CheckOverBuildMenu(sf::RenderWindow &window);
  void DrawBuildingHologram(sf::RenderWindow &window, sf::Vector2f snap_position);
  void ManageHologramColor(bool can_place_building);
  [[nodiscard]] bool const get_hover_build_menu() { return hovering_menu_; };
  [[nodiscard]] const DisplayableBuilding get_current_building_() { return current_selected_building_index_; };
  [[nodiscard]] const NpcType GetNpcTypeBasedOnBuilding();
  void ResetMenu();

  void UpdateWoodValue(int woodAmount);
  void UpdateStoneValue(int stoneAmount);
  void UpdateFoodValue(int foodAmount);

  bool try_to_place_building_ = false;
  [[nodiscard]] const std::span<building::BuildingResource> get_needed_resources_to_build(){return current_needed_resources_;} ;

 private:
  const sf::Vector2f kToggleButtonSize{200, 80};

  const int kCardsNameFontSize = 24;
  const int kCardsResourceFontSize = 20;
  const float kCurrentBuildingSpriteSize = .1f;

  sf::Texture buildings_texture_;

  //BUILD PANEL
  sf::Vector2f position_;
  sf::Font build_menu_font_;
  Button toggle_menu_button_;
  sf::RectangleShape bg_shape_;

  //BUILDING CARDS
  building::BuildingCardUI bcui_lumberjack_;
  building::BuildingCardUI bcui_mine_;
  building::BuildingCardUI bcui_food_picker_;
  building::BuildingCardUI bcui_canteen_;

  //RESOURCES INFOS PANEL
  sf::RectangleShape resources_infos_panel_;
  std::optional<sf::Text> panel_title_text_;
  std::optional<sf::Text> wood_info_label_text_;
  std::optional<sf::Text> wood_info_value_text_;
  std::optional<sf::Text> stone_info_label_text_;
  std::optional<sf::Text> stone_info_value_text_;
  std::optional<sf::Text> food_info_label_text_;
  std::optional<sf::Text> food_info_value_text_;

  //HOLOGRAM
  std::optional<sf::Sprite> current_display_building_sprite_;
  DisplayableBuilding current_selected_building_index_ = DisplayableBuilding::kNone;
  sf::Color building_can_be_placed_color_ = {32, 255, 28, 180};
  sf::Color building_cannot_be_placed_color_ = {255, 77, 28, 180};

  std::vector<building::BuildingResource> current_needed_resources_;

  bool toggle_state_ = false;
  bool hovering_menu_ = false;
  bool place_building_press_ = false;

  void HandleButtonsStates(sf::RenderWindow &window);

  void HandlePanelPosition();

  void SetupToggleButton();

  void SetupBuildingCards();
  void SetupCardsFont();
  void SetupCardsPressEvent();
  void SetupResourceInfosPanel();

  void HandleCardsHover(sf::RenderWindow &window);
  void HandleCardsInput(const std::optional<sf::Event> &event);

  void ChangeSelectedBuildingSprite(int spriteIndex);
  void DisplaySelectedBuilding(sf::RenderWindow &window, sf::Vector2f snap_position);
  void ResetSelectedBuilding();
  void ChangeNeededResourceBasedOnSelectedBuilding(const std::span<building::BuildingResource> neededResources);
};
}
#endif //SAE_ALEXK_CITYBUILDER_API_SRC_MENU_BUILD_MENU_H_
