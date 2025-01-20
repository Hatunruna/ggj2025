#include "GameHub.h"

#include "ProcGen.h"

#include "config.h"

namespace be {

  GameHub::GameHub()
  : GameManager("Bubble Express", { be::GameDataDirectory })
  , start(*this)
  , world(*this)
  {
    state = generateNewGame(random);
    pushScene(start);
  }

}
