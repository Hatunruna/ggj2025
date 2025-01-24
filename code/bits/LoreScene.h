#ifndef BE_LORE_SCENE_H
#define BE_LORE_SCENE_H

#include <gf/Clock.h>
#include <gf/Scene.h>

#include <future>

#include "LoreEndEntity.h"
#include "LoreEntity.h"

namespace be {
  struct GameHub;

  class LoreScene : public gf::Scene {
  public:
    LoreScene(GameHub& game);

    void startLoad();
    bool isLoaded();

  private:
    void doHandleActions(gf::Window& window) override;
    void doUpdate(gf::Time time) override;


    GameHub& m_game;

    gf::Action m_nextAction;

    gf::Clock m_clock;
    bool m_generation_finished = false;
    std::future<void> m_result;

    LoreEntity m_loreEntity;
    LoreEndEntity m_loreEndEntity;
  };

}

#endif // BE_LORE_SCENE_H
