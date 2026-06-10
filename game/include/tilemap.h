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

enum class BackgroundTiles {
  kGround,
  kGrass,
  kFlowerOne,
  kFlowerTwo
};

class Tilemap {
 public:
  void Setup(sf::Vector2f gridSize, sf::Vector2f gridOffset, int seed = 1067);
  void Draw(sf::RenderWindow &window);
  void AddBuilding(DisplayableBuilding building_to_place, sf::Vector2f building_position);

  [[nodiscard]] graphics::Tile &get_tile(int id) { return tiles_[id]; }
  [[nodiscard]] int get_tile_id(int col, int row) const { return row * cols_ + col; }
  [[nodiscard]] std::vector<sf::Vector2f> get_walkables() const;

  [[nodiscard]] sf::Vector2f SnapToGridCenter(sf::Vector2f world_position) const ;
  [[nodiscard]] sf::Vector2f SnapToGridOrigin(sf::Vector2f world_position) const ;
  [[nodiscard]] bool IsTileWalkable(sf::Vector2f world_position) const;

 private:
  graphics::TilemapRenderer ground_renderer_;
  graphics::TilemapRenderer resources_renderer_;
  graphics::TilemapRenderer buildings_renderer_;
  graphics::TileSheet<BackgroundTiles> ground_tile_sheet_;
  graphics::TileSheet<ResourcesType> resources_tile_sheet_;
  graphics::TileSheet<DisplayableBuilding> buildings_tile_sheet_;

  std::vector<graphics::Tile> tiles_;

  sf::Vector2f map_size_;
  sf::Vector2f grid_offset_;
  int cols_ = 0;
  int rows_ = 0;

  [[nodiscard]] int get_sample_index(int sampleSize, int percent) const;
  void AddResourcesTileBasedOnBiome(sf::Vector2f pos, sf::Vector2f gridOffset, Biome::Biome biome);
  void InitTiles();
};

#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_GRAPHICS_TILEMAP_H_
