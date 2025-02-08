#ifndef BE_BASE_SCENE_H
#define BE_BASE_SCENE_H

#include <gf/Scene.h>

namespace be {
  struct GameHub;

  class BaseScene : public gf::Scene {
  public:
    BaseScene(GameHub& game);

  private:
    void doHandleActions(gf::Window& window) override;

    GameHub& m_game;

    gf::Action m_fullscreenAction;
  };

}

#endif // BE_BASE_SCENE_H
