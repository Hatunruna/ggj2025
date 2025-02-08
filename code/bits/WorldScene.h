#ifndef BE_WORLD_SCENE_H
#define BE_WORLD_SCENE_H

#include <SFML/Audio.hpp>
#include <gf/Scene.h>
#include <gf/Views.h>

#include "BubbleEntity.h"
#include "CityEntity.h"
#include "FeedbackEntity.h"
#include "GateEntity.h"
#include "HeroEntity.h"
#include "HeroHintEntity.h"
#include "HeroHudEntity.h"
#include "MapEntity.h"
#include "MountainsEntity.h"
#include "PhysicsDebug.h"
#include "TrapEntity.h"

namespace be {
  struct GameHub;

  class WorldScene : public gf::Scene {
  public:
    WorldScene(GameHub& game);

  private:
    void doProcessEvent(gf::Event& event) override;
    void doHandleActions(gf::Window& window) override;
    void doUpdate(gf::Time time) override;
    void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;
    void doShow() override;

    GameHub& m_game;

    gf::Action m_upAction;
    gf::Action m_leftAction;
    gf::Action m_downAction;
    gf::Action m_rightAction;
    gf::Action m_takeAction;
    gf::Action m_releaseAction;
    gf::Action m_miniMapAction;

    gf::Action m_debugAction;

    MapEntity m_mapEntity;
    TrapEntity m_trapEntity;
    HeroEntity m_heroEntity;
    HeroHintEntity m_heroHintEntity;
    BubbleEntity m_bubbleEntity;
    CityEntity m_cityEntity;
    MountainsEntity m_mountainsEntity;
    GateEntity m_gateEntity;

    HeroHudEntity m_heroHudEntity;
    FeedbackEntity m_feedbackEntity;

    PhysicsDebug m_physicsDebug;

    gf::ZoomingViewAdaptor m_adaptor;

    sf::Music& m_ambianceMusic;
  };

}

#endif // BE_WORLD_SCENE_H
