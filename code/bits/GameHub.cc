#include "GameHub.h"

#include "ProcGen.h"

#include "config.h"

namespace be {

  GameHub::GameHub()
  : GameManager("Bubble Express", { be::GameDataDirectory })
  , state(generateNewGame(random, resources))
  , start(*this)
  , world(*this)
  {
    pushScene(start);
  }

}
