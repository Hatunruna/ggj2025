#ifndef BE_WORLD_SCENE_H
#define BE_WORLD_SCENE_H

#include <gf/Scene.h>
#include <gf/Views.h>

#include "BubbleEntity.h"
#include "HeroEntity.h"
#include "MapEntity.h"

namespace be {
  struct GameHub;

  class WorldScene : public gf::Scene {
  public:
    WorldScene(GameHub& game);

  private:
    void doProcessEvent(gf::Event& event) override;

    GameHub& m_game;
    MapEntity m_mapEntity;
    HeroEntity m_heroEntity;
    BubbleEntity m_bubbleEntity;

    gf::ZoomingViewAdaptor m_adaptor;

  };

}

#endif // BE_WORLD_SCENE_H
