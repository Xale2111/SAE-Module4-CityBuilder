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

enum class BackgroundTiles {
  kGround,
  kGrass,
  kFlowerOne,
  kFlowerTwo
};

enum class ResourcesTiles {
  kWood,
  kStone,
  kFood,
};

enum class BuildingsTiles{
  kLumberjack,
  kMinor,
  kGatherer,
  kFoodHall
};


class Tilemap{
 private :
  graphics::TilemapRenderer groundRenderer_;
  graphics::TilemapRenderer resourcesRenderer_;
  graphics::TilemapRenderer buildingsRenderer_;
  graphics::TileSheet<BackgroundTiles> groundTileSheet_;
  graphics::TileSheet<ResourcesTiles> resourcesTileSheet_;
  graphics::TileSheet<BuildingsTiles> buildingsTileSheet_;

  int GetSampleIndex(int sampleSize,int percent);

  void AddResourcesTileBasedOnBiome(sf::Vector2f gridSize, sf::Vector2f gridOffset, Biome::Biome biome);



 public :
  void Setup(sf::Vector2f gridSize, sf::Vector2f gridOffset, int seed = 1067);
  void Draw(sf::RenderWindow &window);


};
/*
void GenerateTilemap()
{
  FastNoiseLite noise;

  noise.GetNoise(1.f,1.f);
}
*/

#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_GRAPHICS_TILEMAP_H_
