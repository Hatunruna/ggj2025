#ifndef BE_WORLD_SCENE_H
#define BE_WORLD_SCENE_H

#include <gf/Scene.h>
#include <gf/Views.h>

#include "BubbleEntity.h"
#include "CityEntity.h"
#include "FeedbackEntity.h"
#include "HeroEntity.h"
#include "HeroHudEntity.h"
#include "MapEntity.h"
#include "PhysicsDebug.h"

namespace be {
  struct GameHub;

  class WorldScene : public gf::Scene {
  public:
    WorldScene(GameHub& game);

  private:
    void doProcessEvent(gf::Event& event) override;
    void doHandleActions(gf::Window& window) override;
    void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;
    void doShow() override;

    GameHub& m_game;

    gf::Action m_upAction;
    gf::Action m_leftAction;
    gf::Action m_downAction;
    gf::Action m_rightAction;
    gf::Action m_takeAction;
    gf::Action m_releaseAction;

    gf::Action m_debugAction;

    MapEntity m_mapEntity;
    HeroEntity m_heroEntity;
    BubbleEntity m_bubbleEntity;
    CityEntity m_cityEntity;

    HeroHudEntity m_heroHudEntity;
    FeedbackEntity m_feedbackEntity;

    PhysicsDebug m_physicsDebug;

    gf::ZoomingViewAdaptor m_adaptor;

  };

}

#endif // BE_WORLD_SCENE_H
