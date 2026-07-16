//
// Created by alexk on 13.07.2026.
//

#include "saver/saver.h"
#include "resource/resource.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include "data_utils.h"

namespace saver {

void Saver::SaveGame(ResourceAmounts playerResources,
                     std::span<const resource::Resource> resourceMap,
                     std::span<const BuildingInfos> placedBuildings) {

  std::filesystem::create_directories("saves");

  nlohmann::json saveData;

  saveData["version"] = "1.0";

  saveData["player_resources"] = SerializePlayerResources(playerResources);

  saveData["world_resources"] = SerializeWorldResources(resourceMap);

  saveData["buildings"] = SerializeBuildings(placedBuildings);

  try {
    std::ofstream file(savePath);
    if (!file.is_open()) {
      std::cerr << "Couldn't open the file " << savePath << " to write\n";
      return;
    }

    file << saveData.dump(4);
    file.close();

  } catch (const std::exception &e) {
    std::cerr << "Error during save: " << e.what() << "\n";
  }
}

bool Saver::LoadGame(int &collectedWood, int &collectedStone, int &collectedFood,
                     std::vector<resource::Resource> &resourceMap,
                     std::vector<BuildingInfos> &placedBuildings) {

  std::ifstream file(savePath);
  if (!file.is_open()) {
    std::cerr << "No save file found in " << savePath << "\n";
    return false;
  }

  nlohmann::json saveData;
  try {
    file >> saveData;
    file.close();
  }
  catch (const std::exception &e) {
    return false;
  }
  if(saveData.empty())
  {
    return false;
  }

  if (saveData.contains("version")) {
    std::string version = saveData["version"];
    std::cout << "Loading save version " << version << "\n";
  } else {
    return false;
  }

  if (saveData.contains("player_resources")) {
    DeserializePlayerResources(collectedWood, collectedStone, collectedFood, saveData["player_resources"]);
  } else {
    return false;
  }

  if (saveData.contains("world_resources")) {
    DeserializeWorldResources(resourceMap, saveData["world_resources"]);
  } else {
    return false;
  }

  if (saveData.contains("buildings")) {
    DeserializeBuildings(placedBuildings, saveData["buildings"]);
  } else {
    return false;
  }
  return true;
}

nlohmann::json Saver::SerializePlayerResources(ResourceAmounts playerResources) {
  nlohmann::json json;
  json["wood"] = playerResources.wood;
  json["stone"] = playerResources.stone;
  json["food"] = playerResources.food;
  return json;
}

nlohmann::json Saver::SerializeWorldResources(std::span<const resource::Resource> resourceMap) {
  nlohmann::json worldRes = nlohmann::json::array();

  for (const auto &resource : resourceMap) {
    if (resource.type == ResourcesType::kNone) {
      continue;
    }
    nlohmann::json res;
    res["type"] = static_cast<int>(resource.type);
    switch (resource.get_state()) {
      case ResourceState::kGrowing:res["state"] = static_cast<int>(ResourceState::kGrowing);
        break;
      case ResourceState::kReady:
      case ResourceState::kOccupied:
      default:res["state"] = static_cast<int>(ResourceState::kReady);
        break;
    }
    res["x"] = resource.get_pos().x;
    res["y"] = resource.get_pos().y;

    worldRes.push_back(res);
  }

  return worldRes;
}

nlohmann::json Saver::SerializeBuildings(std::span<const BuildingInfos> placedBuildings) {
  nlohmann::json buildings = nlohmann::json::array();

  for (const auto &building : placedBuildings) {
    nlohmann::json b;
    b["type"] = static_cast<int>(building.type);
    b["x"] = building.x;
    b["y"] = building.y;

    buildings.push_back(b);
  }

  return buildings;
}

void Saver::DeserializePlayerResources(int &collectedWood,
                                       int &collectedStone,
                                       int &collectedFood,
                                       const nlohmann::json &json) {
  if (json.contains("wood")) {
    collectedWood = json["wood"].get<int>();
  }
  if (json.contains("stone")) {
    collectedStone = json["stone"].get<int>();
  }
  if (json.contains("food")) {
    collectedFood = json["food"].get<int>();
  }
}

void Saver::DeserializeWorldResources(std::vector<resource::Resource> &resourceMap, const nlohmann::json &json) {
  resourceMap.clear();
  resourceMap.reserve(DataUtils::kTilemapWidth * DataUtils::kTilemapHeight);

  for (int col = 0; col < DataUtils::kTilemapWidth; col++) {
    for (int row = 0; row < DataUtils::kTilemapHeight; row++) {
      int world_x = col * DataUtils::kTileSize;
      int world_y = row * DataUtils::kTileSize;
      resourceMap.emplace_back(sf::Vector2i(world_x, world_y), ResourcesType::kNone);
    }
  }

  for (const auto &resJson : json) {
    int x = resJson["x"].get<int>();
    int y = resJson["y"].get<int>();
    ResourcesType type = static_cast<ResourcesType>(resJson["type"].get<int>());

    int col = x / DataUtils::kTileSize;
    int row = y / DataUtils::kTileSize;

    int id = row * DataUtils::kTilemapWidth + col;

    if (id >= 0 && id < resourceMap.size()) {
      resourceMap[id] = resource::Resource(sf::Vector2i(x, y), type);
    }
  }
}

void Saver::DeserializeBuildings(std::vector<BuildingInfos> &placedBuildings, const nlohmann::json &json) {
  placedBuildings.clear();

  for (const auto &buildingJson : json) {
    BuildingInfos building;
    building.type = static_cast<DisplayableBuilding>(buildingJson["type"].get<int>());
    building.x = buildingJson["x"].get<int>();
    building.y = buildingJson["y"].get<int>();

    placedBuildings.push_back(building);
  }
}
bool Saver::DoesSaveFileExists() {
  return std::filesystem::exists(savePath);
}

} // namespace saver
