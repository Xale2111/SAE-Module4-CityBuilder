//
// Created by alexk on 06.05.2026.
//

#include "game.h"
#include "graphics/tilemap_renderer.h"
#include "graphics/tilesheet.h"
#include "tilemap.h"
#include "menu/menu.h"
#include "camera/camera.h"

namespace game {
namespace {
sf::Clock clock;
sf::RenderWindow window_;

Tilemap tilemap_;
menu::menu mainMenu_;
Camera camera_;
}

void Setup() {

  // Create the main window
  //window_.create(sf::VideoMode({1920, 1080}), "City Builder de fou malade avec des explosions !!",sf::State::Fullscreen);
  window_.create(sf::VideoMode({DataUtils::kScreenWidth, DataUtils::kScreenHeight}),
                 "City Builder de fou malade avec des explosions !!");
  srand(time(NULL));
  tilemap_.Setup({DataUtils::kTilemapWidth, DataUtils::kTilemapHeight}, {64, 64});
  camera_.SetupView({DataUtils::kScreenWidth, DataUtils::kScreenHeight},
                    {DataUtils::kTilemapWidth / 2, DataUtils::kTilemapHeight / 2});
}

ActionCode LoopMenu() {
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
    window_.clear(sf::Color{192, 255, 255});
    mainMenu_.Draw(window_);
    window_.display();
  }

  return ActionCode::kQuit;
}

ActionCode LoopGame() {
  Setup();

  window_.setView(camera_.GetView());

  // Start the game loop
  while (window_.isOpen()) {
    auto dt = clock.restart().asSeconds();

    // Process events = Input frame
    while (const std::optional event = window_.pollEvent()) {
      // Close window: exit
      if (event->is<sf::Event::Closed>()) {
        window_.close();
      }
      camera_.HandleMouse(*event, window_);

    }

    // Graphic frame
    window_.clear();
    tilemap_.Draw(window_);
    window_.display();
  }

  return ActionCode::kMenu;
}
} // namespace game