#include "StartScene.h"

#include <gf/Color.h>

#include "GameHub.h"

namespace be {

  StartScene::StartScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_startAction("Start")
  , m_titleEntity(game.resources)
  {
    setClearColor(gf::Color::Black);

//     m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
//     m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::B);
//     m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::X);
//     m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Y);
    m_startAction.addScancodeKeyControl(gf::Scancode::Space);
    addAction(m_startAction);

    addHudEntity(m_titleEntity);
  }

  void StartScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
      return;
    }

    if (m_startAction.isActive()) {
      m_game.lore.startLoad();
      m_game.replaceScene(m_game.lore); // , m_game.blackout, gf::seconds(TransitionDelay));
    }
  }

}
