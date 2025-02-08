#include "GameHub.h"

#include "config.h"

namespace be {

  GameHub::GameHub()
  : GameManager("Bubble Express", { be::GameDataDirectory })
  , audio({ be::GameDataDirectory })
  , base(*this)
  , start(*this)
  , lore(*this)
  , contract(*this)
  , world(*this)
  , miniMap(*this)
  {
    gf::Ref<gf::Scene> scenes[] = { base, start };
    pushScenes(scenes);
  }

}
