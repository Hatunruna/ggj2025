#include "WorldScene.h"

#include "GameHub.h"

namespace be {

  WorldScene::WorldScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_mapEntity(game)
  , m_heroEntity(game)
  , m_bubbleEntity(game)
  , m_adaptor(game.getRenderer(), getWorldView())
  {
    setClearColor(gf::Color::Black);

    addModel(game.state);

    addWorldEntity(m_mapEntity);
    addWorldEntity(m_heroEntity);
    addWorldEntity(m_bubbleEntity);

    setWorldViewSize({ 512, 512 });
    setWorldViewCenter(game.state.hero.location);
  }

  void WorldScene::doProcessEvent(gf::Event& event)
  {
    m_adaptor.processEvent(event);
  }

}
