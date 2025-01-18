#include "GameHub.h"

#include "config.h"

namespace xy {

  GameHub::GameHub()
  : GameManager("Game", { xy::GameDataDirectory })
  , start(*this)
  {
    pushScene(start);
  }

}
