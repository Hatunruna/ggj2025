#include "BaseScene.h"

#include "GameHub.h"

namespace be {

  BaseScene::BaseScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_fullscreenAction("Fullsceen")
  , m_quitAction("Quit")
  {
    m_fullscreenAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Start);
    m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
    addAction(m_fullscreenAction);

    m_quitAction.addScancodeKeyControl(gf::Scancode::Escape);
    addAction(m_quitAction);
  }

  void BaseScene::doHandleActions(gf::Window& window)
  {
    if (m_fullscreenAction.isActive()) {
      window.toggleFullscreen();
    }

    if (m_quitAction.isActive()) {
      m_game.popAllScenes();
    }
  }

}
