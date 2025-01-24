#ifndef BE_LORE_SCENE_H
#define BE_LORE_SCENE_H

#include <gf/Scene.h>

#include "LoreEntity.h"

namespace be {
  struct GameHub;

  class LoreScene : public gf::Scene {
  public:
    LoreScene(GameHub& game);

  private:
    void doHandleActions(gf::Window& window) override;


    GameHub& m_game;

    gf::Action m_nextAction;

    LoreEntity m_loreEntity;
  };

}

#endif // BE_LORE_SCENE_H
