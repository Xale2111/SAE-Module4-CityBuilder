//
// Created by alexk on 27.05.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_MENU_INCLUDE_MENU_H_
#define SAE_ALEXK_CITYBUILDER_MENU_INCLUDE_MENU_H_

#include <string>
#include "SFML/Graphics.hpp"
#include "button.h"
#include "data_utils.h"

namespace menu {

class menu {
 public:
  std::string world_name_;

  void Start(sf::RenderWindow &window);
  void Draw(sf::RenderWindow &window);
  void HandleButtonsStates(sf::RenderWindow &window);
  [[nodiscard]] ActionCode HandleInput(const std::optional<sf::Event> &event);

 private :
  Button start_button_;
  Button quit_button_;
  sf::Font menu_font_;

  void SetupStartButton();
  void SetupQuitButton();
};

}

#endif //SAE_ALEXK_CITYBUILDER_MENU_INCLUDE_MENU_H_
