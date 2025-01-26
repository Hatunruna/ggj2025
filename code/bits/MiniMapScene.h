#ifndef MINI_MAP_SCENE_H
#define MINI_MAP_SCENE_H

#include <gf/Scene.h>

#include "MiniMapEntity.h"

namespace be {
  struct GameHub;

  class MiniMapScene : public gf::Scene {
  public:
    MiniMapScene(GameHub& game);

    void initMap();

  private:
    void doHandleActions(gf::Window& window) override;
    void doShow() override;

    GameHub& m_game;

    gf::Action m_exitAction;

    MiniMapEntity m_miniMapEntity;
  };

}

#endif // MINI_MAP_SCENE_H
