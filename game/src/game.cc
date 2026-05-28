//
// Created by alexk on 06.05.2026.
//

#include "game.h"
#include "../../api/include/graphics/tilemap_renderer.h"
#include "../../api/include/graphics/tilesheet.h"
#include "tilemap.h"
#include "menu/menu.h"

namespace game {

sf::Clock clock;
sf::RenderWindow window_;

Tilemap tilemap_;
menu::menu mainMenu_;

const int kScreenWidth = 1920;
const int kScreenHeight = 1080;

void Setup() {

  // Create the main window
  //window_.create(sf::VideoMode({1920, 1080}), "City Builder de fou malade avec des explosions !!",sf::State::Fullscreen);
  window_.create(sf::VideoMode({kScreenWidth, kScreenHeight}), "City Builder de fou malade avec des explosions !!");
  srand(time(NULL));
  tilemap_.Setup({6400,6400}, {64,64});
}

ActionCode LoopMenu()
{
  mainMenu_.Start(window_);
  ActionCode actionValue = ActionCode::kMenu;

  while (window_.isOpen()) {

    // Process events = Input frame
    while (const std::optional event = window_.pollEvent()) {
      // Close window: exit
      if (event->is<sf::Event::Closed>()) {
        window_.close();
        return ActionCode::kQuit;
      }
      actionValue = mainMenu_.HandleInput(*event);
      if (actionValue != ActionCode::kMenu)
        return actionValue;
    }

    mainMenu_.HandleButtonsStates(window_);

    // Graphic frame
    window_.clear(sf::Color{192,255,255});
    mainMenu_.Draw(window_);
    window_.display();
  }

  return ActionCode::kQuit;
}
//TODO : Déplacer tout ce qui est relatif à la view dans une class
ActionCode LoopGame() {
  Setup();

  sf::View view({kScreenWidth/2,kScreenHeight/2},{kScreenWidth,kScreenHeight});
  float zoomFactor = .9f;
  float dezoomFactor = 1.1f;
  /*TODO :
   * rajouter un maxZoom et un minZoom
   * se baser sur le zoom actuel pour définir la vitesse du drag
   *
   * */

  bool isDragging_ = false;
  sf::Vector2i lastMousePos_;

  window_.setView(view);

  // Start the game loop
  while (window_.isOpen()) {
    auto dt = clock.restart().asSeconds();

    // Process events = Input frame
    while (const std::optional event = window_.pollEvent()) {
      // Close window: exit
      if (event->is<sf::Event::Closed>()) {
        window_.close();
      }
      if(const auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>())
      {
        float delta = scroll->delta;

        if(delta > 0)
        {
          view.zoom(zoomFactor);
        }
        else
        {
          view.zoom(dezoomFactor);
        }
        window_.setView(view);
      }
      if (const auto* press = event->getIf<sf::Event::MouseButtonPressed>())
      {
        if (press->button == sf::Mouse::Button::Left)
        {
          isDragging_ = true;
          lastMousePos_ = sf::Mouse::getPosition(window_);
        }
      }

      if (const auto* release = event->getIf<sf::Event::MouseButtonReleased>())
      {
        if (release->button == sf::Mouse::Button::Left)
          isDragging_ = false;
      }

      if (const auto* move = event->getIf<sf::Event::MouseMoved>())
      {
        if (isDragging_)
        {
          sf::Vector2i currentPos = sf::Mouse::getPosition(window_);
          sf::Vector2i delta = currentPos - lastMousePos_;

          view.move(sf::Vector2f(-delta.x, -delta.y)); // négatif = sens naturel du drag
          lastMousePos_ = currentPos;

          window_.setView(view);
        }
      }

    }

    // Graphic frame
    window_.clear();
    tilemap_.Draw(window_);
    window_.display();
  }

  return ActionCode::kMenu;
}
} // namespace game