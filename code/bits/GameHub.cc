#include "GameHub.h"

#include "config.h"

namespace be {

  GameHub::GameHub()
  : GameManager("Bubble Express", { be::GameDataDirectory })
  , audio({ be::GameDataDirectory })
  , start(*this)
  , lore(*this)
  , contract(*this)
  , world(*this)
  , miniMap(*this)
  {
    pushScene(start);
  }

}
