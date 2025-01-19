#ifndef be_GAME_HUB_H
#define be_GAME_HUB_H

#include <gf/GameManager.h>

#include "GameData.h"
#include "GameState.h"
#include "StartScene.h"

namespace be {

  struct GameHub : gf::GameManager {
    GameHub();

    GameData data;
    GameState state;

    StartScene start;
  };


}


#endif // be_GAME_HUB_H
