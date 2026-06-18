//
// Created by alexk on 06.05.2026.
//

#include "game.h"
#include "tilemap.h"
#include "menu/menu.h"
#include "camera/camera.h"
#include "menu/build_menu.h"
#include "ai/npc.h"
#include "ai/npc_manager.h"

namespace game {

namespace {
sf::Clock clock;
sf::RenderWindow window_;

Tilemap tilemap_;
menu::Menu mainMenu_;
menu::BuildMenu build_menu_;
camera::Camera camera_;

api::ai::NpcManager npc_manager_;

}

void Setup() {

  window_.setFramerateLimit(60);
  window_.setVerticalSyncEnabled(true);

  // Create the main window
  //window_.create(sf::VideoMode({1920, 1080}), "City Builder de fou malade avec des explosions !!",sf::State::Fullscreen);
  window_.create(sf::VideoMode({DataUtils::kScreenWidth, DataUtils::kScreenHeight}),
                 "City Builder de fou malade avec des explosions !!");

  srand(time(0));

  tilemap_.Setup();
  float realMapWidth = DataUtils::kTilemapWidth * DataUtils::kTileSize;
  float realMapHeight = DataUtils::kTilemapHeight * DataUtils::kTileSize;

  camera_.SetupView({DataUtils::kScreenWidth, DataUtils::kScreenHeight},
                    {realMapWidth / 2.0f, realMapHeight / 2.0f});

  npc_manager_.SpawnNpc("_assets/tempPNJ.png",{realMapWidth / 2.0f, realMapHeight / 2.0f});
}

ActionCode LoopMenu() {
  mainMenu_.Init(window_);
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
  build_menu_.Init();


  // Init the game loop
  while (window_.isOpen()) {

    auto dt = clock.restart().asSeconds();

    //TODO : Optimize so this only play when hologram
    sf::Vector2f mouse_world = window_.mapPixelToCoords(
        sf::Mouse::getPosition(window_), camera_.GetView());
    sf::Vector2f snapped = tilemap_.SnapToGridCenter(mouse_world);
    bool can_place = tilemap_.IsTileWalkable(snapped);

    build_menu_.ManageHologramColor(can_place);


    // Process events = Input frame
    while (const std::optional event = window_.pollEvent()) {
      // Close window: exit
      if (event->is<sf::Event::Closed>()) {
        build_menu_.ResetMenu();
        window_.close();
      }

      build_menu_.HandleMenu(window_, *event);
      build_menu_.CheckOverBuildMenu(window_);

      if (!build_menu_.get_hover_build_menu()) {
        camera_.HandleMouse(*event, window_);
      }

    }

    if (build_menu_.try_to_place_building_) {
      if (build_menu_.get_current_building_() != DisplayableBuilding::kNone && can_place) {
        snapped = tilemap_.SnapToGridOrigin(mouse_world);
        tilemap_.AddBuilding(build_menu_.get_current_building_(), snapped);
      }
      build_menu_.try_to_place_building_ = false;
    }

    npc_manager_.UpdatePath(tilemap_.get_walkables());
    npc_manager_.Update(dt);

    // Graphic frame
    window_.clear();
    //Drawing game related elements
    window_.setView(camera_.GetView());
    tilemap_.Draw(window_);
    build_menu_.DrawBuildingHologram(window_, snapped);
    npc_manager_.Draw(window_);

    //Drawing UI related elements
    window_.setView(window_.getDefaultView());
    build_menu_.Draw(window_);
    window_.display();
  }

  return ActionCode::kMenu;
}
} // namespace game