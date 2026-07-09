//
// Created by alexk on 04.06.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_API_INCLUDE_BUILDING_BUILDING_RESOURCE_H_
#define SAE_ALEXK_CITYBUILDER_API_INCLUDE_BUILDING_BUILDING_RESOURCE_H_



namespace building{

#include "data_utils.h"

struct BuildingResource
{
  int amount_;
  const ResourcesType type_;

};

//Lumberjack needed resources : 500 stone, 600 food
constexpr BuildingResource kLumberjackNeededStoneAmount = {resource::kStonePerRockAmount*5, ResourcesType::kStone};   //Need to chop down 5 rocks
constexpr BuildingResource kLumberjackNeededFoodAmount = {resource::kFoodPerFieldAmount*3, ResourcesType::kFood};     //Need to chop down 3 field

//Picker needed resources : 600 wood, 300 stone
constexpr BuildingResource kPickerNeededWoodAmount = {resource::kWoodPerTreeAmount*4, ResourcesType::kWood};          //Need to chop down 4 trees
constexpr BuildingResource kPickerNeededStoneAmount = {resource::kStonePerRockAmount*3, ResourcesType::kStone};       //Need to chop down 3 rocks

//Minor needed resources : 750 wood
constexpr BuildingResource kMinorNeededWoodAmount = {resource::kWoodPerTreeAmount*5, ResourcesType::kWood};           //Need to chop down 5 trees

//Canteen needed resources : 3000 of everything
constexpr BuildingResource kCanteenNeededWoodAmount = {3000, ResourcesType::kWood};       //Need 20 trees
constexpr BuildingResource kCanteenNeededStoneAmount = {3000, ResourcesType::kStone};     //Need 30 rocks
constexpr BuildingResource kCanteenNeededFoodAmount = {3000, ResourcesType::kFood};       //Need 15 fields

/*
 * If wanted to be *10 of everything (issue is that the text will be too long)
constexpr BuildingResource kCanteenNeededWoodAmount = {resource::kWoodPerTreeAmount*10, ResourcesType::kWood};       //Need 20 trees
constexpr BuildingResource kCanteenNeededStoneAmount = {resource::kStonePerRockAmount*10, ResourcesType::kStone};     //Need 30 rocks
constexpr BuildingResource kCanteenNeededFoodAmount = {resource::kFoodPerFieldAmount*10, ResourcesType::kFood};       //Need 15 fields
*/

}

#endif //SAE_ALEXK_CITYBUILDER_API_INCLUDE_BUILDING_BUILDING_RESOURCE_H_
