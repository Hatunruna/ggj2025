#ifndef BE_GAME_HUB_H
#define BE_GAME_HUB_H

#include <gf/GameManager.h>

#include "GameData.h"
#include "GameState.h"
#include "ContractScene.h"
#include "LoreScene.h"
#include "StartScene.h"
#include "WorldScene.h"

namespace be {

  struct GameHub : gf::GameManager {
    GameHub();

    GameData data;
    GameState state;

    StartScene start;
    LoreScene lore;
    ContractScene contract;
    WorldScene world;
  };


}


#endif // BE_GAME_HUB_H
