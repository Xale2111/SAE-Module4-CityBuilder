//
// Created by alexk on 27.05.2026.
//

#include <print>
#include "menu/menu.h"
#include "menu/button.h"

namespace menu {

constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 600;
constexpr int buttonWidth = 300;
constexpr int buttonHeight = 180;

void Menu::Init(sf::RenderWindow &window) {
  if (!menu_font_.openFromFile("_assets/fonts/ShareTech-Regular.ttf")) {
    std::println(stderr, "Failed to load main menu font");
  }

  if (!unito_texture_.loadFromFile("_assets/unito.png")) {
    std::println(stderr, "Failed to load unito logo");
  }

  unito_logo_ = sf::Sprite(unito_texture_);
  unito_logo_->setScale({.55f, .55f});
  auto logoSize = unito_logo_->getGlobalBounds().size;
  unito_logo_->setPosition({kScreenWidth-logoSize.x, kScreenHeight-logoSize.y});

  window.create(sf::VideoMode({kScreenWidth, kScreenHeight}), "City Builder de fou malade avec des explosions !!");
  SetupStartButton();
  SetupQuitButton();
}

void Menu::Draw(sf::RenderWindow &window) {
  start_button_.Draw(window);
  quit_button_.Draw(window);
  window.draw(*unito_logo_);
}

void Menu::HandleButtonsStates(sf::RenderWindow &window) {
  start_button_.CheckHover(sf::Vector2f(sf::Mouse::getPosition(window)));
  start_button_.HandleState();

  quit_button_.CheckHover(sf::Vector2f(sf::Mouse::getPosition(window)));
  quit_button_.HandleState();
}

ActionCode Menu::HandleInput(const std::optional<sf::Event> &event) {

  start_button_.HandleInput(event);
  quit_button_.HandleInput(event);

  if (start_button_.has_been_pressed()) {
    start_button_.ResetPressState();
    return start_button_.get_action_code();
  }

  if (quit_button_.has_been_pressed()) {
    quit_button_.ResetPressState();
    return quit_button_.get_action_code();
  }

  return ActionCode::kMenu;
}
void Menu::SetupStartButton() {
  start_button_.set_font(menu_font_, 50);
  start_button_.set_font_color(sf::Color::Black);
  start_button_.set_font_hover_color(sf::Color::White);
  start_button_.set_text("Play");
  start_button_.set_size({buttonWidth, buttonHeight});
  start_button_.set_position({kScreenWidth * .25, kScreenHeight * .25});
  start_button_.set_fill_color(sf::Color{65, 171, 93});
  start_button_.set_hover_color(sf::Color{35, 139, 69});
  start_button_.set_pressed_color(sf::Color{0, 90, 50});
  start_button_.set_action_code(ActionCode::kPlay);
  start_button_.ResetPressState();
}

void Menu::SetupQuitButton() {

  quit_button_.set_font(menu_font_, 50);
  quit_button_.set_font_color(sf::Color::Black);
  quit_button_.set_font_hover_color(sf::Color::White);
  quit_button_.set_text("Quit");
  quit_button_.set_size({buttonWidth, buttonHeight});
  quit_button_.set_position({kScreenWidth * .25, kScreenHeight * .75});
  quit_button_.set_fill_color(sf::Color{65, 171, 93});
  quit_button_.set_hover_color(sf::Color{35, 139, 69});
  quit_button_.set_pressed_color(sf::Color{0, 90, 50});
  quit_button_.set_action_code(ActionCode::kQuit);
  quit_button_.ResetPressState();
}
}