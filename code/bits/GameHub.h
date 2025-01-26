#ifndef BE_GAME_HUB_H
#define BE_GAME_HUB_H

#include <gf/GameManager.h>

#include "AudioManager.h"
#include "GameData.h"
#include "GameState.h"
#include "ContractScene.h"
#include "LoreScene.h"
#include "StartScene.h"
#include "WorldScene.h"
#include "MiniMapScene.h"

namespace be {

  struct GameHub : gf::GameManager {
    GameHub();

    GameData data;
    GameState state;

    AudioManager audio;

    StartScene start;
    LoreScene lore;
    ContractScene contract;
    WorldScene world;
    MiniMapScene miniMap;
  };


}


#endif // BE_GAME_HUB_H
