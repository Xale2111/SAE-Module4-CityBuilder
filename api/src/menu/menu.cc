//
// Created by alexk on 27.05.2026.
//

#include <iostream>
#include "menu/menu.h"
#include "menu/button.h"

namespace menu {

constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 600;
constexpr int buttonWidth = 300;
constexpr int buttonHeight = 180;


void menu::Start(sf::RenderWindow& window) {
  if(!menuFont.openFromFile("_assets/fonts/ShareTech-Regular.ttf"))
  {
    std::cout << "Menu font didn't load" << std::endl;
  }

  window.create(sf::VideoMode({kScreenWidth, kScreenHeight}), "City Builder de fou malade avec des explosions !!");
  SetupStartButton();
  SetupQuitButton();
}

void menu::Draw(sf::RenderWindow &window) {
  startButton.Draw(window);
  quitButton.Draw(window);
}

void menu::HandleButtonsStates(sf::RenderWindow &window) {
  startButton.CheckHover(sf::Vector2f (sf::Mouse::getPosition(window)));
  startButton.HandleState();

  quitButton.CheckHover(sf::Vector2f (sf::Mouse::getPosition(window)));
  quitButton.HandleState();
}

ActionCode menu::HandleInput(const std::optional<sf::Event>& event) {
  startButton.HandleInput(event);
  quitButton.HandleInput(event);

  if(startButton.HasBeenPressed())
  {
    startButton.ResetPressState();
    return startButton.GetActionCode();
  }

  if(quitButton.HasBeenPressed())
  {
    quitButton.ResetPressState();
    return quitButton.GetActionCode();
  }

  return ActionCode::kMenu;
}
void menu::SetupStartButton() {
  startButton.SetFont(menuFont, 50);
  startButton.SetFontColor(sf::Color::Black);
  startButton.SetFontHoverColor(sf::Color::White);
  startButton.SetText("Play");
  startButton.SetSize({buttonWidth,buttonHeight});
  startButton.SetPosition({kScreenWidth*.25,kScreenHeight*.25});
  startButton.SetFillColor(sf::Color{128,255,255});
  startButton.SetHoverColor(sf::Color{0,153,184});
  startButton.SetPressColor(sf::Color{0,102,128});
  startButton.SetActionCode(ActionCode::kPlay);
  startButton.ResetPressState();
}

void menu::SetupQuitButton() {

  quitButton.SetFont(menuFont, 50);
  quitButton.SetFontColor(sf::Color::Black);
  quitButton.SetFontHoverColor(sf::Color::White);
  quitButton.SetText("Quit");
  quitButton.SetSize({buttonWidth,buttonHeight});
  quitButton.SetPosition({kScreenWidth*.25,kScreenHeight*.75});
  quitButton.SetFillColor(sf::Color{128,255,255});
  quitButton.SetHoverColor(sf::Color{0,153,184});
  quitButton.SetPressColor(sf::Color{0,102,128});
  quitButton.SetActionCode(ActionCode::kQuit);
  quitButton.ResetPressState();
}
}