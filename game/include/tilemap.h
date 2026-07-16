//
// Created by alexk on 07.05.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_INCLUDE_GRAPHICS_TILEMAP_H_
#define SAE_ALEXK_CITYBUILDER_API_INCLUDE_GRAPHICS_TILEMAP_H_

#include <SFML/Graphics/RenderWindow.hpp>
#include <span>
#include "FastNoiseLite.h"
#include "graphics/tilemap_renderer.h"
#include "graphics/tilesheet.h"
#include "biome.h"
#include <random>
#include "data_utils.h"
#include <ranges>
#include <unordered_set>
#include "resource/resource_manager.h"

enum class BackgroundTiles {
  kGround,
  kGrass,
  kFlowerOne,
  kFlowerTwo
};

class Tilemap {
 public:
  void Setup(resource::ResourceManager& rscManager, int seed = 1067);
  void ReconstructMapAfterLoad(resource::ResourceManager& rscManager);
  void Draw(sf::RenderWindow &window);
  void AddBuilding(DisplayableBuilding building_to_place, sf::Vector2f building_position);

  [[nodiscard]] std::vector<sf::Vector2i> &get_walkables() { return walkables_; }
  [[nodiscard]] int get_tile_id(int col, int row) const { return row * total_cols_ + col; }

  [[nodiscard]] sf::Vector2f SnapToGridCenter(sf::Vector2f world_position) const ;
  [[nodiscard]] sf::Vector2f SnapToGridOrigin(sf::Vector2f world_position) const ;
  [[nodiscard]] bool IsTileWalkable(sf::Vector2f world_position) const;

  [[nodiscard]] std::span<const BuildingInfos> get_placed_buildings() {return placed_buildings_;} ;
  [[nodiscard]] std::vector<BuildingInfos>& get_placed_buildings_ref() {return placed_buildings_;} ;

  void OnResourceHarvested(int resource_index);
  void OnResourceRespawned(int resource_index);

  //void DebugWalkable();

 private:
  graphics::TilemapRenderer ground_renderer_;
  graphics::TilemapRenderer resources_renderer_;
  graphics::TilemapRenderer buildings_renderer_;
  graphics::TileSheet<BackgroundTiles> ground_tile_sheet_;
  graphics::TileSheet<ResourcesType> resources_tile_sheet_;
  graphics::TileSheet<DisplayableBuilding> buildings_tile_sheet_;

  std::vector<BuildingInfos> placed_buildings_;

  std::vector<graphics::Tile> tiles_;
  std::vector<sf::Vector2i> walkables_;
  resource::ResourceManager* resource_manager_ = nullptr;

  const sf::Vector2f grid_offset_ = {DataUtils::kTileSize, DataUtils::kTileSize};
  const int total_cols_ = DataUtils::kTilemapWidth;
  const int total_rows_ = DataUtils::kTilemapHeight;

  [[nodiscard]] int get_sample_index(int sampleSize, int percent) const;
  void AddResourcesTileBasedOnBiome(sf::Vector2f pos, sf::Vector2f gridOffset, Biome::Biome biome);
  void InitTiles();
  void InitReconstructedTiles();


  void RebuildResources(sf::Vector2f pos, sf::Vector2f gridOffset, ResourcesType type);
  void RebuildBuilding(DisplayableBuilding building_to_place, sf::Vector2f building_position);

  bool InitGroundTileSheet();
  bool InitResourcesTileSheet();
  bool InitBuildingTileSheet();
  void SetWalkablesBasedOnTiles();

};

#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_GRAPHICS_TILEMAP_H_
