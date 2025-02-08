#include "BaseScene.h"

#include "GameHub.h"

namespace be {

  BaseScene::BaseScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_fullscreenAction("fullsceen")
  {
    m_fullscreenAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Start);
    m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
    addAction(m_fullscreenAction);
  }

  void BaseScene::doHandleActions(gf::Window& window)
  {
    if (m_fullscreenAction.isActive()) {
      window.toggleFullscreen();
    }
  }

}
