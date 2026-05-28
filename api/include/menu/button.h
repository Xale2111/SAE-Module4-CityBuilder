//
// Created by alexk on 27.05.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_MENU_SRC_BUTTON_H_
#define SAE_ALEXK_CITYBUILDER_MENU_SRC_BUTTON_H_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "DataUtils.h"


namespace menu{
class Button
{
 private:
  sf::RectangleShape shape_;
  std::optional<sf::Text> text_;

  sf::Color fillColor_;
  sf::Color pressColor_;
  sf::Color hoverColor_;

  sf::Color textColor_;
  sf::Color textHoverColor_;

  bool isPressed_ = false;
  bool hasBeenPressed_ = false;
  bool isMouseHover_ = false;

  ActionCode actionCode_;

 public:
  void Draw(sf::RenderWindow &window) const;


  void HandleInput(const std::optional<sf::Event>& event);
  bool CheckHover(sf::Vector2f mousePosition);
  void HandleState();

  bool HasBeenPressed();
  void ResetPressState();

  void SetText(std::string text);
  void SetSize(sf::Vector2f size);
  void SetFillColor(sf::Color color);
  void SetHoverColor(sf::Color color);
  void SetPressColor(sf::Color color);
  void SetPosition(sf::Vector2f position);
  void SetOutline(int size, sf::Color color);

  void SetFont(sf::Font& font,int size);
  void SetFontColor(sf::Color color);
  void SetFontHoverColor(sf::Color color);

  void SetActionCode(ActionCode action);
  ActionCode GetActionCode();
};

}

#endif //SAE_ALEXK_CITYBUILDER_MENU_SRC_BUTTON_H_
