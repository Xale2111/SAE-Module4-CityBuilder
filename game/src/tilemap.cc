//
// Created by alexk on 20.05.2026.
//

#include <iostream>
#include "tilemap.h"
#include "data_utils.h"
#include <print>

std::random_device r;
std::default_random_engine rng_(r());

void Tilemap::Setup(sf::Vector2f gridSize, sf::Vector2f gridOffset, int seed) {

  grid_offset_ = gridOffset;
  total_cols_ = static_cast<int>(gridSize.x);
  total_rows_ = static_cast<int>(gridSize.y);

  InitTiles();

  if (resources_tile_sheet_.InitTileSheet("_assets/resources.png", 512)) {
    resources_tile_sheet_.AddTile(ResourcesType::kWood, 2, 0);
    resources_tile_sheet_.AddTile(ResourcesType::kStone, 0, 1);
    resources_tile_sheet_.AddTile(ResourcesType::kFood, 1, 2);

    resources_renderer_.SetTexture(resources_tile_sheet_.GetTexture());
    resources_renderer_.Clear();

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

    std::vector<float> samples;
    samples.reserve(total_cols_ * total_rows_);

    for (int col = 0; col < total_cols_; col++)
      for (int row = 0; row < total_rows_; row++)
        samples.push_back((noiseBiome.GetNoise(col * gridOffset.x, row * gridOffset.y) + 1.0f) * 0.5f);

    std::sort(samples.begin(), samples.end());
    int n = static_cast<int>(samples.size());

    float t1 = samples[get_sample_index(n, woodPercent)];
    float t2 = samples[get_sample_index(n, stonePercent)];
    float t3 = samples[get_sample_index(n, foodPercent)];

    for (int col = 0; col < total_cols_; col++) {
      for (int row = 0; row < total_rows_; row++) {
        sf::Vector2f pos = {col * gridOffset.x, row * gridOffset.y};
        float biomeValue = (noiseBiome.GetNoise(pos.x, pos.y) + 1.0f) * 0.5f;

        int biome = 0;
        if (biomeValue < t1) biome = 1;
        else if (biomeValue < t2) biome = 2;
        else if (biomeValue < t3) biome = 3;

        switch (biome) {
          case 1: AddResourcesTileBasedOnBiome(pos, gridOffset, Biome::kForest);
            break;
          case 2: AddResourcesTileBasedOnBiome(pos, gridOffset, Biome::kQuarry);
            break;
          case 3: AddResourcesTileBasedOnBiome(pos, gridOffset, Biome::kField);
            break;
          default: break;
        }
      }
    }
  }

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
        sf::Vector2f pos = {col * gridOffset.x, row * gridOffset.y};
        float noiseValue = std::abs(noise.GetNoise(pos.x, pos.y));

        if (noiseValue <= .2f) {
          ground_renderer_.AddTile(pos, gridOffset, ground_tile_sheet_.GetBounds(BackgroundTiles::kGround));
        } else if (noiseValue <= .5f) {
          ground_renderer_.AddTile(pos, gridOffset, ground_tile_sheet_.GetBounds(BackgroundTiles::kGrass));
        } else {
          std::uniform_int_distribution<int> dist(0, 2);
          if (dist(rng_) % 2 == 0)
            ground_renderer_.AddTile(pos, gridOffset, ground_tile_sheet_.GetBounds(BackgroundTiles::kFlowerOne));
          else
            ground_renderer_.AddTile(pos, gridOffset, ground_tile_sheet_.GetBounds(BackgroundTiles::kFlowerTwo));
        }
      }
    }
  }

  if (buildings_tile_sheet_.InitTileSheet("_assets/buildings.png", 512)) {
    buildings_tile_sheet_.AddTile(DisplayableBuilding::kFoodHouse, 0, 0);
    buildings_tile_sheet_.AddTile(DisplayableBuilding::kLumberjackHouse, 1, 0);
    buildings_tile_sheet_.AddTile(DisplayableBuilding::kMineHouse, 2, 0);
    buildings_tile_sheet_.AddTile(DisplayableBuilding::kCanteen, 3, 0);

    buildings_renderer_.SetTexture(buildings_tile_sheet_.GetTexture());
    buildings_renderer_.Clear();
  }

  UpdateWalkables();
}

void Tilemap::InitTiles() {
  tiles_.resize(total_cols_ * total_rows_);
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
      resources_renderer_.AddTile(pos, gridOffset, resources_tile_sheet_.GetBounds(static_cast<ResourcesType>(i - 1)));

      // Marque la tile comme non walkable
      int col = static_cast<int>(pos.x / gridOffset.x);
      int row = static_cast<int>(pos.y / gridOffset.y);
      tiles_[get_tile_id(col, row)].is_walkable = false;

      return;
    }
  }
}

void Tilemap::UpdateWalkables() {
  walkables_.clear();
  for (const auto& tile : tiles_) {
    if (tile.is_walkable) {
      walkables_.push_back(tile);
    }
  }
}


/*
void Tilemap::DebugWalkable() {
  for (int row = 0; row < total_rows_; ++row) {
    for (int col = 0; col < total_cols_; col++) {
      int index = get_tile_id(col, row);
      std::println("Tile position : {},{}  |  index : {}", col, row, index);
    }
  }
}*/

