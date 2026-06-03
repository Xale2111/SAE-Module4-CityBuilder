#include <game.h>
#include "menu/menu.h"

int main() {

  ActionCode actionCode = ActionCode::kMenu;

  do {
    switch (actionCode) {
      case ActionCode::kMenu:actionCode = game::LoopMenu();
        break;
      case ActionCode::kPlay:actionCode = game::LoopGame();
        break;
      default:
        //Nothing to do
        break;
    }
  } while (actionCode != ActionCode::kQuit);

}

