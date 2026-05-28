//
// Created by alexk on 20.05.2026.
//

#include <iostream>
#include "tilemap.h"
#include <print>

std::random_device r;
std::default_random_engine rng_(r());

void Tilemap::Setup(sf::Vector2f gridSize, sf::Vector2f gridOffset, int seed)

{

  FastNoiseLite noise;
  noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
  noise.SetSeed(1337);
  noise.SetFrequency(0.01f);

  if (groundTileSheet_.InitTileSheet("_assets/ground.png", 512)) {
    groundTileSheet_.AddTile(BackgroundTiles::kGround, 0, 0);
    groundTileSheet_.AddTile(BackgroundTiles::kGrass, 3, 0);
    groundTileSheet_.AddTile(BackgroundTiles::kFlowerOne, 1, 0);
    groundTileSheet_.AddTile(BackgroundTiles::kFlowerTwo, 2, 0);

    // init textures
    groundRenderer_.SetTexture(groundTileSheet_.GetTexture());
    groundRenderer_.Clear();


    for (float x = 0.f; x < gridSize.x ; x+=gridOffset.x) {
      for (float y = 0.f; y < gridSize.y; y+=gridOffset.y) {
        float noiseValue = std::abs(noise.GetNoise(x, y)); //noise value is between -1 and 1

        if(noiseValue <= .2f)
        {
          groundRenderer_.AddTile({x, y}, gridOffset, groundTileSheet_.GetBounds(BackgroundTiles::kGround));
        }
        else if(noiseValue <= .5f)
        {
          groundRenderer_.AddTile({x, y}, gridOffset, groundTileSheet_.GetBounds(BackgroundTiles::kGrass));
        } else
        {
          std::uniform_int_distribution<int> dist(0,2);
          int randomNumber = dist(rng_);

          if(randomNumber%2 == 0)
          {
            groundRenderer_.AddTile({x, y}, gridOffset, groundTileSheet_.GetBounds(BackgroundTiles::kFlowerOne));
          }
          else
          {
            groundRenderer_.AddTile({x, y}, gridOffset, groundTileSheet_.GetBounds(BackgroundTiles::kFlowerTwo));
          }
        }

      }

    }
  }


  if(resourcesTileSheet_.InitTileSheet("_assets/resources.png", 512))
  {
    resourcesTileSheet_.AddTile(ResourcesTiles::kWood, 2, 0);
    resourcesTileSheet_.AddTile(ResourcesTiles::kStone, 0, 1);
    resourcesTileSheet_.AddTile(ResourcesTiles::kFood, 1, 2);

    resourcesRenderer_.SetTexture(resourcesTileSheet_.GetTexture());
    resourcesRenderer_.Clear();

    // Bruit 1 : définit les zones de biomes (inchangé)
    FastNoiseLite noiseBiome;
    noiseBiome.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    noiseBiome.SetSeed(seed);
    noiseBiome.SetFrequency(0.002f);
    noiseBiome.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);
    noiseBiome.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Euclidean);
    noiseBiome.SetCellularJitter(0.67f);
    noiseBiome.SetFractalType(FastNoiseLite::FractalType_None); // pas de fractale ici

    constexpr int woodPercent = 30 ;
    constexpr int stonePercent = 23 + woodPercent;
    constexpr int foodPercent = 17  + stonePercent;

    std::vector<float> samples;
    samples.reserve((gridSize.x / gridOffset.x) * (gridSize.y / gridOffset.y));

    for (float x = 0.f; x < gridSize.x; x += gridOffset.x)
      for (float y = 0.f; y < gridSize.y; y += gridOffset.y)
        samples.push_back((noiseBiome.GetNoise(x, y) + 1.0f) * 0.5f);

    std::sort(samples.begin(), samples.end());
    int n = samples.size();

    float t1 = samples[GetSampleIndex(n, woodPercent)];
    float t2 = samples[GetSampleIndex(n, stonePercent)];
    float t3 = samples[GetSampleIndex(n, foodPercent)];

    // Passe 2 : placement
    for (float x = 0.f; x < gridSize.x; x += gridOffset.x) {
      for (float y = 0.f; y < gridSize.y; y += gridOffset.y) {

        float biomeValue  = (noiseBiome.GetNoise(x, y) + 1.0f) * 0.5f;

        // Define biome (0 = empty, 1 = wood, 2 = stone, 3 = food)
        int biome = 0; // empty
        if      (biomeValue < t1) biome = 1; // wood
        else if (biomeValue < t2) biome = 2; // stone
        else if (biomeValue < t3) biome = 3; // food

        //Once we have the biome, if it's not empty, we need to randomly choose the resource based on a percentage
        switch(biome) {
          case 1:
            AddResourcesTileBasedOnBiome( {x,y}, gridOffset, Biome::forest);
            break;
          case 2:
            AddResourcesTileBasedOnBiome( {x,y}, gridOffset, Biome::quarry);
            break;
          case 3:
            AddResourcesTileBasedOnBiome( {x,y}, gridOffset, Biome::field);
            break;
          default: break;
        }
      }
    }
  }
}

void Tilemap::Draw(sf::RenderWindow &window)
{
  groundRenderer_.Draw(window);
  resourcesRenderer_.Draw(window);
}
int Tilemap::GetSampleIndex(int sampleSize, int percent) {
  return static_cast<int>(std::clamp(sampleSize * percent / 100, 0, sampleSize - 1));
}

void Tilemap::AddResourcesTileBasedOnBiome(sf::Vector2f pos, sf::Vector2f gridOffset, Biome::Biome biome)
{
  std::uniform_int_distribution<int> dist(0, 99);
  int randomNumber = dist(rng_);

  // Vide ?
  if (randomNumber < biome.emptyPercentage) return;

  float cumulative = biome.emptyPercentage;
  for (int i = 1; i < 4; i++)
  {
    cumulative += biome.percentages[i];
    if (randomNumber < cumulative)
    {
      resourcesRenderer_.AddTile(pos, gridOffset,resourcesTileSheet_.GetBounds(static_cast<ResourcesTiles>(i - 1)));
      return;
    }
  }
}
