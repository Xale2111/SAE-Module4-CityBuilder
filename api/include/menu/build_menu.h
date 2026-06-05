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
  void HandleInput(const std::optional<sf::Event> &event);
  void CheckOverBuildMenu(sf::RenderWindow &window);
  [[nodiscard]] const bool get_hover_build_menu(){return hovering_menu_;};
  [[nodiscard]] const DisplayableBuilding get_current_building_(){return current_selected_building_index_;};

 private:
  const sf::Vector2f kToggleButtonSize{200, 80};

  const int kCardsNameFontSize = 24;
  const int kCardsResourceFontSize = 20;
  const float kCurrentBuildingSpriteSize = .1f;

  sf::Texture buildings_texture_;

  sf::Vector2f position_;
  sf::Font build_menu_font_;
  Button toggle_menu_button_;
  sf::RectangleShape bg_shape_;

  building::BuildingCardUI bcui_lumberjack_;
  building::BuildingCardUI bcui_mine_;
  building::BuildingCardUI bcui_food_picker_;
  building::BuildingCardUI bcui_canteen_;

  std::optional<sf::Sprite> current_display_building_sprite_;
  DisplayableBuilding current_selected_building_index_= DisplayableBuilding::kNone;

  bool toggle_state_ = false;
  bool hovering_menu_ = false;
  bool place_building_press_ = false;

  void HandleButtonsStates(sf::RenderWindow &window);

  void HandlePanelPosition();

  void SetupToggleButton();

  void SetupBuildingCards();
  void SetupCardsFont();
  void SetupCardsPressEvent();

  void HandleCardsHover(sf::RenderWindow &window);
  void HandleCardsInput(const std::optional<sf::Event> &event);

  void ChangeSelectedBuildingSprite(int spriteIndex);
  void DisplaySelectedBuilding(sf::RenderWindow &window);
  void ResetSelectedBuilding();
  /*
   * Définir le sprite actuel
   * l'afficher sur la souris
   *
   *
   * */
};
}
#endif //SAE_ALEXK_CITYBUILDER_API_SRC_MENU_BUILD_MENU_H_
