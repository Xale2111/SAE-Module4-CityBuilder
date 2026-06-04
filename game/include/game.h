//
// Created by alexk on 06.05.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_GAME_H
#define SAE_ALEXK_CITYBUILDER_GAME_H
#include <iostream>
#include "SFML/Graphics.hpp"
#include "data_utils.h"


namespace game {

void Setup();
[[nodiscard]] ActionCode LoopMenu();
[[nodiscard]] ActionCode LoopGame();

}

#endif //SAE_ALEXK_CITYBUILDER_GAME_H
