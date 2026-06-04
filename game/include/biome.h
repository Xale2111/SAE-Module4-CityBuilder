//
// Created by alexk on 21.05.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_GAME_INCLUDE_BIOME_H_
#define SAE_ALEXK_CITYBUILDER_GAME_INCLUDE_BIOME_H_
namespace Biome {

struct Biome {
  const float empty_percentage_ = 100;
  const float wood_percentage_ = 0;
  const float stone_percentage_ = 0;
  const float food_percentage_ = 0;
  const std::array<const float, 4>
      percentages = {empty_percentage_, wood_percentage_, stone_percentage_, food_percentage_};
};

constexpr Biome kPlain = {91.f, 3, 3, 3};
constexpr Biome kForest = {50.f, 35.f, 2.f, 5.f};
constexpr Biome kQuarry = {60.f, 5.f, 25.f, 2.f};
constexpr Biome kField = {55.f, 2.f, 5.f, 30.f};
}

#endif //SAE_ALEXK_CITYBUILDER_GAME_INCLUDE_BIOME_H_

