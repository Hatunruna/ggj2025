#include "GameHub.h"

#include "ProcGen.h"

#include "config.h"

namespace be {

  GameHub::GameHub()
  : GameManager("Bubble Express", { be::GameDataDirectory })
  , start(*this)
  , lore(*this)
  , contract(*this)
  , world(*this)
  {
    pushScene(start);
  }

}
