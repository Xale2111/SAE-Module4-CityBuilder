//
// Created by alexk on 27.05.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_MENU_INCLUDE_MENU_H_
#define SAE_ALEXK_CITYBUILDER_MENU_INCLUDE_MENU_H_

#include <string>
#include "SFML/Graphics.hpp"
#include "button.h"
#include "DataUtils.h"

namespace menu {

class menu {
 private :
  Button startButton;
  Button quitButton;
  sf::Font menuFont;

  void SetupStartButton();
  void SetupQuitButton();

 public:
  std::string worldName;
  int screenWidth;
  int screenHeight;

  void Start(sf::RenderWindow &window);
  void Draw(sf::RenderWindow &window);
  void HandleButtonsStates(sf::RenderWindow &window);
  ActionCode HandleInput(const std::optional<sf::Event>& event);
};

}

#endif //SAE_ALEXK_CITYBUILDER_MENU_INCLUDE_MENU_H_
