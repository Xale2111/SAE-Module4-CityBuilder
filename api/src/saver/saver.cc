//
// Created by alexk on 13.07.2026.
//

#include "saver/saver.h"
#include "resource/resource.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace saver {

void Saver::SaveGame(ResourceAmounts playerResources,
                     std::span<const resource::Resource> resourceMap,
                     std::span<const BuildingInfos> placedBuildings,
                     const std::string& filename) {
  

  std::filesystem::create_directories("saves");
  
  std::string savePath = "saves/" + filename;
  
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
    
  } catch (const std::exception& e) {
    std::cerr << "Error during save: " << e.what() << "\n";
  }
}

bool Saver::LoadGame(ResourceAmounts& playerResources,
                     std::vector<resource::Resource>& resourceMap,
                     std::vector<BuildingInfos>& placedBuildings,
                     const std::string& filename) {
  
  try {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "No save file found in " << filename << "\n";
      return false;
    }
    
    nlohmann::json saveData;
    file >> saveData;
    file.close();
    

    if (saveData.contains("version")) {
      std::string version = saveData["version"];
      std::cout << "Loading save version " << version << "\n";
    }

    if (saveData.contains("player_resources")) {
      DeserializePlayerResources(playerResources, saveData["player_resources"]);
    }

    if (saveData.contains("world_resources")) {
      DeserializeWorldResources(resourceMap, saveData["world_resources"]);
    }

    if (saveData.contains("buildings")) {
      DeserializeBuildings(placedBuildings, saveData["buildings"]);
    }
    
    
    return true;
    
  } catch (const std::exception& e) {
    std::cerr << "Error during loading: " << e.what() << "\n";
    return false;
  }
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
  
  for (const auto& resource : resourceMap) {
    nlohmann::json res;
    res["type"] = static_cast<int>(resource.type);
    res["state"] = static_cast<int>(resource.get_state());
    res["x"] = resource.get_pos().x;
    res["y"] = resource.get_pos().y;
    
    worldRes.push_back(res);
  }
  
  return worldRes;
}

nlohmann::json Saver::SerializeBuildings(std::span<const BuildingInfos> placedBuildings) {
  nlohmann::json buildings = nlohmann::json::array();
  
  for (const auto& building : placedBuildings) {
    nlohmann::json b;
    b["type"] = static_cast<int>(building.type);
    b["x"] = building.x;
    b["y"] = building.y;
    
    buildings.push_back(b);
  }
  
  return buildings;
}

void Saver::DeserializePlayerResources(ResourceAmounts& playerResources, const nlohmann::json& json) {
  if (json.contains("wood")) {
    playerResources.wood = json["wood"].get<int>();
  }
  if (json.contains("stone")) {
    playerResources.stone = json["stone"].get<int>();
  }
  if (json.contains("food")) {
    playerResources.food = json["food"].get<int>();
  }
}

void Saver::DeserializeWorldResources(std::vector<resource::Resource>& resourceMap, const nlohmann::json& json) {
  resourceMap.clear();
  
  for (const auto& resJson : json) {
    ResourcesType type = static_cast<ResourcesType>(resJson["type"].get<int>());
    ResourceState state = static_cast<ResourceState>(resJson["state"].get<int>());
    int x = resJson["x"].get<int>();
    int y = resJson["y"].get<int>();

    resource::Resource res(sf::Vector2i(x, y), type);
    
    resourceMap.push_back(res);
  }
}

void Saver::DeserializeBuildings(std::vector<BuildingInfos>& placedBuildings, const nlohmann::json& json) {
  placedBuildings.clear();
  
  for (const auto& buildingJson : json) {
    BuildingInfos building;
    building.type = static_cast<DisplayableBuilding>(buildingJson["type"].get<int>());
    building.x = buildingJson["x"].get<int>();
    building.y = buildingJson["y"].get<int>();
    
    placedBuildings.push_back(building);
  }
}

} // namespace saver
