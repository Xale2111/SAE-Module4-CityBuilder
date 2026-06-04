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
#include "data_utils.h"
#include <functional>
#include <vector>

namespace menu {

class Event {
 public:
  void operator+=(std::function<void()> callback) {
    callbacks_.push_back(callback);
  }
  void Invoke() {
    for (auto& callback : callbacks_)
      callback();
  }
 private:
  std::vector<std::function<void()>> callbacks_;
};

class Button {
 public:
  Event OnPress;

  void Draw(sf::RenderWindow &window) const;

  void HandleInput(const std::optional<sf::Event> &event);
  void CheckHover(sf::Vector2f mousePosition);
  void HandleState();

  [[nodiscard]] bool has_been_pressed() const;
  void ResetPressState();

  void set_text(std::string_view new_text);
  void set_size(sf::Vector2f new_size);
  void set_fill_color(sf::Color new_color);
  void set_hover_color(sf::Color new_color);
  void set_pressed_color(sf::Color new_color);
  void set_position(sf::Vector2f new_position);
  void set_outline(int new_size, sf::Color new_color);

  void set_font(sf::Font &new_font, int new_size);
  void set_font_color(sf::Color new_color);
  void set_font_hover_color(sf::Color new_color);

  void set_action_code(ActionCode new_action);
  [[nodiscard]] ActionCode get_action_code() const;

 private:
  sf::RectangleShape shape_;
  std::optional<sf::Text> text_;

  sf::Color fill_color_;
  sf::Color press_color_;
  sf::Color hover_color_;

  sf::Color text_color_;
  sf::Color text_hover_color_;

  bool is_pressed_ = false;
  bool has_been_pressed_ = false;
  bool is_mouse_hover_ = false;

  ActionCode action_code_ = ActionCode::kMenu;

};

}

#endif //SAE_ALEXK_CITYBUILDER_MENU_SRC_BUTTON_H_
