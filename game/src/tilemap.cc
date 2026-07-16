//
// Created by alexk on 20.05.2026.
//

#include <iostream>
#include "tilemap.h"
#include "data_utils.h"
#include <print>
#include <tracy/Tracy.hpp>
#include <unordered_map>
#include <unordered_set>

std::random_device r;
std::default_random_engine rng_(r());

void Tilemap::Setup(resource::ResourceManager &rscManager, int seed) {

  tiles_.resize(total_cols_ * total_rows_);
  walkables_.reserve(total_cols_ * total_rows_);

  resource_manager_ = &rscManager;

  InitTiles();

  if (InitResourcesTileSheet()) {

    FastNoiseLite noiseBiome;
    noiseBiome.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    noiseBiome.SetSeed(seed);
    noiseBiome.SetFrequency(0.002f);
    noiseBiome.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);
    noiseBiome.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Euclidean);
    noiseBiome.SetCellularJitter(0.67f);
    noiseBiome.SetFractalType(FastNoiseLite::FractalType_None);

    constexpr int woodPercent = 30;
    constexpr int stonePercent = 23 + woodPercent;
    constexpr int foodPercent = 17 + stonePercent;

    ZoneScopedNC("Biome", tracy::Color::Cyan);

    std::vector<float> samples;
    samples.reserve(total_cols_ * total_rows_);

    for (int col = 0; col < total_cols_; col++)
      for (int row = 0; row < total_rows_; row++)
        samples.push_back((noiseBiome.GetNoise(col * grid_offset_.x, row * grid_offset_.y) + 1.0f) * 0.5f);

    //TODO : Try with nth_element instead of sort
    //auto it1 = std::nth_element(samples.begin(), samples.begin() +woodPercent, samples.end());

    std::sort(samples.begin(), samples.end());
    int n = static_cast<int>(samples.size());

    float t1 = samples[get_sample_index(n, woodPercent)];
    float t2 = samples[get_sample_index(n, stonePercent)];
    float t3 = samples[get_sample_index(n, foodPercent)];

    for (int col = 0; col < total_cols_; col++) {
      for (int row = 0; row < total_rows_; row++) {
        sf::Vector2f pos = {col * grid_offset_.x, row * grid_offset_.y};
        float biomeValue = (noiseBiome.GetNoise(pos.x, pos.y) + 1.0f) * 0.5f;

        int biome = 0;
        if (biomeValue < t1) biome = 1;
        else if (biomeValue < t2) biome = 2;
        else if (biomeValue < t3) biome = 3;

        switch (biome) {
          case 1: AddResourcesTileBasedOnBiome(pos, grid_offset_, Biome::kForest);
            break;
          case 2: AddResourcesTileBasedOnBiome(pos, grid_offset_, Biome::kQuarry);
            break;
          case 3: AddResourcesTileBasedOnBiome(pos, grid_offset_, Biome::kField);
            break;
          default: break;
        }
      }
    }
  }

  if (!InitGroundTileSheet()) {
    //DISPLAY ERROR
  }

  if (!InitBuildingTileSheet()) {
    //DISPLAY ERROR
  }

  SetWalkablesBasedOnTiles();
}

void Tilemap::ReconstructMapAfterLoad(resource::ResourceManager &rscManager) {
  tiles_.resize(total_cols_ * total_rows_);
  walkables_.reserve(total_cols_ * total_rows_);

  resource_manager_ = &rscManager;

  InitReconstructedTiles();

  if (!InitResourcesTileSheet()) {
    //DISPLAY ERROR
  }
  if (!InitGroundTileSheet()) {
    //DISPLAY ERROR
  }
  if (!InitBuildingTileSheet()) {
    //DISPLAY ERROR
  }

  for (auto &resource : resource_manager_->get_resources()) {
    if (resource.type == ResourcesType::kNone) continue;
    sf::Vector2f pos = {static_cast<float>(resource.get_pos().x), static_cast<float>(resource.get_pos().y)};
    RebuildResources(pos, grid_offset_, resource.type);
  }

  for (auto &building : placed_buildings_) {
    RebuildBuilding(building.type, {static_cast<float>(building.x), static_cast<float>(building.y)});
  }

  SetWalkablesBasedOnTiles();
}

void Tilemap::InitTiles() {
  for (int row = 0; row < total_rows_; row++) {
    for (int col = 0; col < total_cols_; col++) {
      int id = get_tile_id(col, row);
      tiles_[id].position = {col * grid_offset_.x, row * grid_offset_.y};
      tiles_[id].is_walkable = true;
      resource_manager_->AddResource({col * DataUtils::kTileSize, row * DataUtils::kTileSize}, ResourcesType::kNone);
    }
  }
}

void Tilemap::InitReconstructedTiles() {
  for (int col = 0; col < total_cols_; col++) {
    for (int row = 0; row < total_rows_; row++) {
      int id = get_tile_id(col, row);
      tiles_[id].position = {col * grid_offset_.x, row * grid_offset_.y};
      tiles_[id].is_walkable = true;
    }
  }
}

sf::Vector2f Tilemap::SnapToGridCenter(sf::Vector2f world_position) const {
  int col = static_cast<int>(world_position.x / grid_offset_.x);
  int row = static_cast<int>(world_position.y / grid_offset_.y);
  return {col * grid_offset_.x + grid_offset_.x / 2, row * grid_offset_.y + grid_offset_.y / 2};
}

sf::Vector2f Tilemap::SnapToGridOrigin(sf::Vector2f world_position) const {
  int col = static_cast<int>(world_position.x / grid_offset_.x);
  int row = static_cast<int>(world_position.y / grid_offset_.y);
  return {col * grid_offset_.x, row * grid_offset_.y};
}

bool Tilemap::IsTileWalkable(sf::Vector2f world_position) const {
  int col = static_cast<int>(world_position.x / grid_offset_.x);
  int row = static_cast<int>(world_position.y / grid_offset_.y);

  //check we are inside the map
  if (col < 0 || col >= total_cols_ || row < 0 || row >= total_rows_) return false;

  return tiles_[get_tile_id(col, row)].is_walkable;
}

void Tilemap::OnResourceHarvested(int resource_index) {
  auto &resource = resource_manager_->get_resources()[resource_index];
  sf::Vector2f pos = {static_cast<float>(resource.get_pos().x),
                      static_cast<float>(resource.get_pos().y)};

  // Change la texture vers la version récoltée
  ResourcesType new_type = resource.type;

  switch (resource.type) {

    case ResourcesType::kWood:
      new_type = ResourcesType::kGrowingWood;
      break;
    case ResourcesType::kStone:
      new_type = ResourcesType::kGrowingStone;
      break;
    case ResourcesType::kFood:
      new_type = ResourcesType::kGrowingFood;
      break;
  }

  if(resources_renderer_.UpdateTileTexture(pos,resources_tile_sheet_.GetBounds(new_type))) {
    walkables_.push_back(resource.get_pos());
  }
  else
  {
    //DISPLAY ERROR
  }
}

void Tilemap::OnResourceRespawned(int resource_index) {
  auto &resource = resource_manager_->get_resources()[resource_index];
  sf::Vector2f pos = {static_cast<float>(resource.get_pos().x),static_cast<float>(resource.get_pos().y)};

  if(resources_renderer_.UpdateTileTexture(pos,resources_tile_sheet_.GetBounds(resource.type))) {
    std::erase_if(walkables_, [&](const sf::Vector2i &w) {
      return w.x == resource.get_pos().x && w.y == resource.get_pos().y;
    });
  }
  else{
    //DISPLAY ERROR
  }
}

void Tilemap::Draw(sf::RenderWindow &window) {
  ground_renderer_.Draw(window);
  resources_renderer_.Draw(window);
  buildings_renderer_.Draw(window);
}

void Tilemap::AddBuilding(DisplayableBuilding building_to_place, sf::Vector2f building_position) {
  buildings_renderer_.AddTile(building_position, grid_offset_,
                              buildings_tile_sheet_.GetBounds(building_to_place));

  int col = static_cast<int>(building_position.x / grid_offset_.x);
  int row = static_cast<int>(building_position.y / grid_offset_.y);
  tiles_[get_tile_id(col, row)].is_walkable = false;
  walkables_.push_back({static_cast<int>(tiles_[get_tile_id(col, row)].position.x),
                        static_cast<int>(tiles_[get_tile_id(col, row)].position.y)});

  placed_buildings_.push_back({building_to_place, static_cast<int>(building_position.x),
                               static_cast<int>(building_position.y)});
}

int Tilemap::get_sample_index(int sampleSize, int percent) const {
  return static_cast<int>(std::clamp(sampleSize * percent / 100, 0, sampleSize - 1));
}

void Tilemap::AddResourcesTileBasedOnBiome(sf::Vector2f pos, sf::Vector2f gridOffset, Biome::Biome biome) {
  std::uniform_int_distribution<int> dist(0, 99);
  int randomNumber = dist(rng_);

  if (randomNumber < biome.empty_percentage_) return;

  float cumulative = biome.empty_percentage_;
  for (int i = 1; i < 4; i++) {
    cumulative += biome.percentages[i];
    if (randomNumber < cumulative) {

      ResourcesType type = static_cast<ResourcesType>(i - 1);

      resources_renderer_.AddTile(pos, gridOffset, resources_tile_sheet_.GetBounds(type));

      // Marque la tile comme non walkable
      int col = static_cast<int>(pos.x / gridOffset.x);
      int row = static_cast<int>(pos.y / gridOffset.y);
      tiles_[get_tile_id(col, row)].is_walkable = false;
      resource_manager_->SetResourceType(get_tile_id(col, row), type);

      return;
    }
  }
}

void Tilemap::RebuildResources(sf::Vector2f pos, sf::Vector2f gridOffset, ResourcesType type) {
  resources_renderer_.AddTile(pos, gridOffset, resources_tile_sheet_.GetBounds(type));
  int col = static_cast<int>(pos.x / gridOffset.x);
  int row = static_cast<int>(pos.y / gridOffset.y);
  tiles_[get_tile_id(col, row)].is_walkable = false;
}

void Tilemap::RebuildBuilding(DisplayableBuilding building_to_place, sf::Vector2f building_position)
{
  buildings_renderer_.AddTile(building_position, grid_offset_,
                              buildings_tile_sheet_.GetBounds(building_to_place));

  int col = static_cast<int>(building_position.x / grid_offset_.x);
  int row = static_cast<int>(building_position.y / grid_offset_.y);
  tiles_[get_tile_id(col, row)].is_walkable = false;
  walkables_.push_back({static_cast<int>(tiles_[get_tile_id(col, row)].position.x),
                        static_cast<int>(tiles_[get_tile_id(col, row)].position.y)});
}

bool Tilemap::InitGroundTileSheet() {
  if (ground_tile_sheet_.InitTileSheet("_assets/ground.png", 512)) {
    ground_tile_sheet_.AddTile(BackgroundTiles::kGround, 0, 0);
    ground_tile_sheet_.AddTile(BackgroundTiles::kGrass, 3, 0);
    ground_tile_sheet_.AddTile(BackgroundTiles::kFlowerOne, 1, 0);
    ground_tile_sheet_.AddTile(BackgroundTiles::kFlowerTwo, 2, 0);

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetSeed(1337);
    noise.SetFrequency(0.01f);

    ground_renderer_.SetTexture(ground_tile_sheet_.GetTexture());
    ground_renderer_.Clear();

    for (int col = 0; col < total_cols_; col++) {
      for (int row = 0; row < total_rows_; row++) {
        sf::Vector2f pos = {col * grid_offset_.x, row * grid_offset_.y};
        float noiseValue = std::abs(noise.GetNoise(pos.x, pos.y));

        if (noiseValue <= .2f) {
          ground_renderer_.AddTile(pos, grid_offset_, ground_tile_sheet_.GetBounds(BackgroundTiles::kGround));
        } else if (noiseValue <= .5f) {
          ground_renderer_.AddTile(pos, grid_offset_, ground_tile_sheet_.GetBounds(BackgroundTiles::kGrass));
        } else {
          std::uniform_int_distribution<int> dist(0, 2);
          if (dist(rng_) % 2 == 0)
            ground_renderer_.AddTile(pos, grid_offset_, ground_tile_sheet_.GetBounds(BackgroundTiles::kFlowerOne));
          else
            ground_renderer_.AddTile(pos, grid_offset_, ground_tile_sheet_.GetBounds(BackgroundTiles::kFlowerTwo));
        }
      }
    }
    return true;
  }
  return false;
}

bool Tilemap::InitResourcesTileSheet() {
  if (resources_tile_sheet_.InitTileSheet("_assets/resources.png", 512)) {
    resources_tile_sheet_.AddTile(ResourcesType::kWood, 1, 0);
    resources_tile_sheet_.AddTile(ResourcesType::kStone, 1, 1);
    resources_tile_sheet_.AddTile(ResourcesType::kFood, 1, 2);
    resources_tile_sheet_.AddTile(ResourcesType::kGrowingWood, 0, 0);
    resources_tile_sheet_.AddTile(ResourcesType::kGrowingStone, 0,1);
    resources_tile_sheet_.AddTile(ResourcesType::kGrowingFood, 0, 2);


    resources_renderer_.SetTexture(resources_tile_sheet_.GetTexture());
    resources_renderer_.Clear();
    return true;
  }
  return false;
}
bool Tilemap::InitBuildingTileSheet() {
  if (buildings_tile_sheet_.InitTileSheet("_assets/buildings.png", 512)) {
    buildings_tile_sheet_.AddTile(DisplayableBuilding::kFoodHouse, 0, 0);
    buildings_tile_sheet_.AddTile(DisplayableBuilding::kLumberjackHouse, 1, 0);
    buildings_tile_sheet_.AddTile(DisplayableBuilding::kMineHouse, 2, 0);
    buildings_tile_sheet_.AddTile(DisplayableBuilding::kCanteen, 3, 0);

    buildings_renderer_.SetTexture(buildings_tile_sheet_.GetTexture());
    buildings_renderer_.Clear();
    return true;
  }
  return false;
}
void Tilemap::SetWalkablesBasedOnTiles() {
  for (auto &tile : tiles_) {
    if (tile.is_walkable) {
      walkables_.push_back({static_cast<int>(tile.position.x), static_cast<int>(tile.position.y)});
    }
  }
}
