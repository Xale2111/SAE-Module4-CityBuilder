//
// Created by alexk on 27.05.2026.
//

#include "menu/button.h"
#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace menu {

void Button::HandleInput(const std::optional<sf::Event> &event) {
  if (is_mouse_hover_) {
    if (const auto *mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
      is_pressed_ = true;
      event_on_press_.Invoke();
    }
    if (const auto *mouse = event->getIf<sf::Event::MouseButtonReleased>()) {
      is_pressed_ = false;
      has_been_pressed_ = true;
    }
  } else {
    is_pressed_ = false;
  }
}

void Button::CheckHover(sf::Vector2f mousePosition) {
  if (shape_.getGlobalBounds().contains(mousePosition)) {
    is_mouse_hover_ = true;
  } else {
    is_mouse_hover_ = false;
  }
}

void Button::HandleState() {
  if (is_mouse_hover_) {
    if (is_pressed_) {
      shape_.setFillColor(press_color_);
    } else {
      shape_.setFillColor(hover_color_);
    }
    if (text_.has_value()) {
      text_->setFillColor(text_hover_color_);
    }
  } else {
    shape_.setFillColor(fill_color_);
    if (text_.has_value()) {
      text_->setFillColor(text_color_);
    }
  }
}

bool Button::has_been_pressed() const {
  return has_been_pressed_;
}

void Button::ResetPressState() {
  has_been_pressed_ = false;
}

void Button::set_text(std::string_view new_text) {
  if (text_.has_value()) {
    text_->setString(new_text.data());
  }
}

void Button::set_size(sf::Vector2f new_size) {
  shape_.setSize(new_size);
  shape_.setOrigin(sf::Vector2f(new_size.x / 2, new_size.y / 2));
}

void Button::set_fill_color(sf::Color new_color) {
  fill_color_ = new_color;
}

void Button::set_hover_color(sf::Color new_color) {
  hover_color_ = new_color;
}

void Button::set_pressed_color(sf::Color new_color) {
  press_color_ = new_color;
}

void Button::set_position(sf::Vector2f new_position) {
  shape_.setPosition(new_position);
  if (text_.has_value()) {
    float textXOrigin = shape_.getPosition().x - text_->getGlobalBounds().size.x / 2;
    float textYOrigin = shape_.getPosition().y - text_->getGlobalBounds().size.y;
    text_->setPosition({textXOrigin, textYOrigin});
  }
}

void Button::set_outline(int new_size, sf::Color new_color) {
  shape_.setOutlineThickness(new_size);
  shape_.setOutlineColor(new_color);
}

void Button::set_font(sf::Font &new_font, int new_size) {
  text_ = sf::Text(new_font);
  text_->setCharacterSize(new_size);
}

void Button::set_font_color(sf::Color new_color) {
  if (text_.has_value()) {
    text_color_ = new_color;
    text_->setFillColor(text_color_);
  }
}

void Button::set_font_hover_color(sf::Color new_color) {
  if (text_.has_value()) {
    text_hover_color_ = new_color;
    text_->setFillColor(text_hover_color_);
  }
}

void Button::set_action_code(ActionCode new_action) {
  action_code_ = new_action;
}

ActionCode Button::get_action_code() const {
  return action_code_;
}
void Button::Draw(sf::RenderWindow &window) const {
  sf::RenderStates states;
  window.draw(shape_, states);
  if (text_.has_value()) {
    window.draw(*text_, states);
  }
}


}

