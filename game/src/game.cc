//
// Created by alexk on 06.05.2026.
//

#include "game.h"
#include "tilemap.h"
#include "menu/menu.h"
#include "camera/camera.h"
#include "menu/build_menu.h"
#include "ai/npc_manager.h"
#include "resource/resource_manager.h"
#include <tracy/Tracy.hpp>
#include "saver/saver.h"

namespace game {

namespace {
sf::Clock clock;
sf::RenderWindow window_;

Tilemap tilemap_;
menu::Menu mainMenu_;
menu::BuildMenu build_menu_;
camera::Camera camera_;
resource::ResourceManager resource_manager_;

saver::Saver saver_;

api::ai::NpcManager npc_manager_;

constexpr float realMapWidth = DataUtils::kTilemapWidth * DataUtils::kTileSize;
constexpr float realMapHeight = DataUtils::kTilemapHeight * DataUtils::kTileSize;

}

void Setup() {

  window_.setFramerateLimit(60);
  window_.setVerticalSyncEnabled(true);

  // Create the main window
  //window_.create(sf::VideoMode({1920, 1080}), "City Builder de fou malade avec des explosions !!",sf::State::Fullscreen);
  window_.create(sf::VideoMode({DataUtils::kScreenWidth, DataUtils::kScreenHeight}),"City Builder de fou malade avec des explosions !!");
  //window_.create(sf::VideoMode({DataUtils::kScreenWidth, DataUtils::kScreenHeight}),"City Builder de fou malade avec des explosions !!",sf::State::Fullscreen);

  srand(time(0));

  camera_.SetupView({DataUtils::kScreenWidth, DataUtils::kScreenHeight}, {realMapWidth / 2.0f, realMapHeight / 2.0f});

  //npc_manager_.SpawnNpc(NpcType::kGatherer,{realMapWidth / 2.0f, realMapHeight / 2.0f});
  //npc_manager_.SpawnNpc(NpcType::kLumberjack,{realMapWidth / 2.0f, realMapHeight / 2.0f});

  npc_manager_.Reset();

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
    window_.clear();
    mainMenu_.Draw(window_);
    window_.display();
  }

  return ActionCode::kQuit;
}

ActionCode LoopGame() {
    Setup();
  if (saver_.DoesSaveFileExists() && saver_.LoadGame(resource_manager_.get_wood_amount_ref(),resource_manager_.get_stone_amount_ref(),resource_manager_.get_food_amount_ref(),resource_manager_.get_resources_ref(),tilemap_.get_placed_buildings_ref())) {
    tilemap_.ReconstructMapAfterLoad(resource_manager_);
  } else {
    tilemap_.Setup(resource_manager_);
    saver_.SaveGame(resource_manager_.get_all_resources_amount(),resource_manager_.get_resources(),tilemap_.get_placed_buildings());
  }
  build_menu_.Init();


  float delay = 0.0f;
  float time = 0.0f;
  int maxNpc = 100;
  int currentNpc = 0;

  // Init the game loop
  while (window_.isOpen()) {

    auto dt = clock.restart().asSeconds();

/*
    time += dt;
    if (time >= delay && currentNpc < maxNpc)
    {
      time = 0.0f;
      npc_manager_.SpawnNpc(NpcType::kMiner, {realMapWidth / 2.0f, realMapHeight / 2.0f});
      currentNpc++;
    }*/


    //TODO : Optimize so this only play when hologram
    sf::Vector2f mouse_world = window_.mapPixelToCoords(
        sf::Mouse::getPosition(window_), camera_.GetView());
    sf::Vector2f snapped = tilemap_.SnapToGridCenter(mouse_world);
    bool has_needed_resources = resource_manager_.CheckAvailableResource(build_menu_.get_needed_resources_to_build());
    bool can_place = tilemap_.IsTileWalkable(snapped) && has_needed_resources;

    build_menu_.ManageHologramColor(can_place);


    // Process events = Input frame
    while (const std::optional event = window_.pollEvent()) {
      // Close window: exit
      if (event->is<sf::Event::Closed>()) {
        saver_.SaveGame(resource_manager_.get_all_resources_amount(),
                        resource_manager_.get_resources(),
                        tilemap_.get_placed_buildings());
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
      if (has_needed_resources) {
        if (build_menu_.get_current_building_() != DisplayableBuilding::kNone && can_place) {
          snapped = tilemap_.SnapToGridOrigin(mouse_world);
          tilemap_.AddBuilding(build_menu_.get_current_building_(), snapped);
          //Spawn npc at building position
          auto npcToSpawn = build_menu_.GetNpcTypeBasedOnBuilding();
          if (npcToSpawn != NpcType::kNone) {
            npc_manager_.SpawnNpc(npcToSpawn, snapped);
          }
          resource_manager_.UseResource(build_menu_.get_needed_resources_to_build());
        }
      }
      build_menu_.try_to_place_building_ = false;
    }

    resource_manager_.Update(dt);
    resource_manager_.CollectResources(npc_manager_.get_collected_resources());
    npc_manager_.UpdatePath(tilemap_.get_walkables(), resource_manager_.get_resources_ref());
    npc_manager_.Update(dt);

    build_menu_.UpdateWoodValue(resource_manager_.get_wood_amount());
    build_menu_.UpdateStoneValue(resource_manager_.get_stone_amount());
    build_menu_.UpdateFoodValue(resource_manager_.get_food_amount());


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

  FrameMark;

  return ActionCode::kMenu;
}
} // namespace game