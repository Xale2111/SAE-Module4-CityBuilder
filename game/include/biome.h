//
// Created by alexk on 21.05.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_GAME_INCLUDE_BIOME_H_
#define SAE_ALEXK_CITYBUILDER_GAME_INCLUDE_BIOME_H_
namespace Biome {

struct Biome {
  const float emptyPercentage;
  const float woodPercentage;
  const float stonePercentage;
  const float foodPercentage;
  const std::array<const float, 4> percentages = {emptyPercentage,woodPercentage,stonePercentage,foodPercentage};
};

constexpr Biome plain = { 0, 3, 3, 3};
constexpr Biome forest = {50.f, 35.f, 2.f, 5.f};
constexpr Biome quarry = {60.f, 5.f, 25.f, 2.f};
constexpr Biome field = {55.f, 2.f, 5.f, 30.f};
}

#endif //SAE_ALEXK_CITYBUILDER_GAME_INCLUDE_BIOME_H_

