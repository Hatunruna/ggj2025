#include "GameHub.h"

#include "config.h"

namespace be {

  GameHub::GameHub()
  : GameManager("Bubble Express", { be::GameDataDirectory })
  , start(*this)
  {
    pushScene(start);
  }

}
