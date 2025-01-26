#include "MiniMapScene.h"

#include "GameHub.h"
#include "MapSettings.h"

namespace be {

  MiniMapScene::MiniMapScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_exitAction("exit")
  , m_miniMapEntity(game)
  {
    setClearColor(gf::Color::Gray());

    setWorldViewSize(MapSize);
    setWorldViewCenter({ 0.0f, 0.0f });

    m_exitAction.addScancodeKeyControl(gf::Scancode::Escape);
    addAction(m_exitAction);

    addWorldEntity(m_miniMapEntity);
  }

  void MiniMapScene::initMap()
  {
    m_miniMapEntity.updateMiniMap();
  }

  void MiniMapScene::doShow()
  {
    m_miniMapEntity.updateFogOfWar();
  }

  void MiniMapScene::doHandleActions([[maybe_unused]] gf::Window& window)
  {
    if (m_exitAction.isActive()) {
      m_game.replaceScene(m_game.world);
    }
  }

}
