#include "LoreScene.h"

#include "GameHub.h"

namespace be {

  LoreScene::LoreScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_nextAction("next")
  , m_loreEntity(game)
  {
    setClearColor(gf::Color::Black);

    m_nextAction.addScancodeKeyControl(gf::Scancode::Space);
    m_nextAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
    addAction(m_nextAction);

    addHudEntity(m_loreEntity);
  }

  void LoreScene::doHandleActions(gf::Window& window)
  {
    if (m_nextAction.isActive()) {
      m_game.replaceAllScenes(m_game.contract);
    }
  }

}
