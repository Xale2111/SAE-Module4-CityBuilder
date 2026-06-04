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

enum class BuildingsTiles {
  kLumberjack,
  kMinor,
  kGatherer,
  kFoodHall
};

class Tilemap {
 public :
  void Setup(sf::Vector2f gridSize, sf::Vector2f gridOffset, int seed = 1067);
  void Draw(sf::RenderWindow &window);

 private :
  graphics::TilemapRenderer ground_renderer_;
  graphics::TilemapRenderer resources_renderer_;
  graphics::TilemapRenderer buildings_renderer_;
  graphics::TileSheet<BackgroundTiles> ground_tile_sheet_;
  graphics::TileSheet<ResourcesType> resources_tile_sheet_;
  graphics::TileSheet<BuildingsTiles> buildings_tile_sheet_;

  [[nodiscard]] int GetSampleIndex(int sampleSize, int percent) const;

  void AddResourcesTileBasedOnBiome(sf::Vector2f gridSize, sf::Vector2f gridOffset, Biome::Biome biome);



};
#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_GRAPHICS_TILEMAP_H_
