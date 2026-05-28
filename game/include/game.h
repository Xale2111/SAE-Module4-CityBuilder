//
// Created by alexk on 06.05.2026.
//

#ifndef SAE_ALEXK_CITYBUILDER_GAME_H
#define SAE_ALEXK_CITYBUILDER_GAME_H
#include <iostream>
#include "SFML/Graphics.hpp"
#include "DataUtils.h"

namespace game {
void Setup();
ActionCode LoopMenu();
ActionCode LoopGame();

}

#endif //SAE_ALEXK_CITYBUILDER_GAME_H
