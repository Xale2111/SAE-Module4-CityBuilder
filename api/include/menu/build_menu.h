//
// Created by alexk on 04.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_SRC_MENU_BUILD_MENU_H_
#define SAE_ALEXK_CITYBUILDER_API_SRC_MENU_BUILD_MENU_H_

#include "SFML/Graphics.hpp"
#include "button.h"
#include "building/building_ui_card.h"

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

 private:
  const sf::Vector2f toggle_button_size_{200, 80};

  sf::Vector2f position_;
  sf::Font build_menu_font_;
  Button toggle_menu_button_;
  sf::RectangleShape bg_shape_;

  building::BuildingCardUI bcui_lumberjack_;
  building::BuildingCardUI bcui_mine_;
  building::BuildingCardUI bcui_food_picker_;
  building::BuildingCardUI bcui_canteen_;

  bool toggle_state_ = false;

  void HandleButtonsStates(sf::RenderWindow &window);
  void HandleInput(const std::optional<sf::Event> &event);

  void HandlePanelPosition();

  void SetupToggleButton();

  void SetupBuildingCards();

};
}
#endif //SAE_ALEXK_CITYBUILDER_API_SRC_MENU_BUILD_MENU_H_
