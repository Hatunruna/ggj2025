#include "LoreScene.h"

#include <gf/Log.h>

#include "GameHub.h"
#include "ProcGen.h"

namespace be {

  LoreScene::LoreScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_nextAction("next")
  , m_loreEntity(game)
  , m_loreEndEntity(game)
  {
    setClearColor(gf::Color::Black);

    m_nextAction.addScancodeKeyControl(gf::Scancode::Space);
    m_nextAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
    addAction(m_nextAction);

    addHudEntity(m_loreEntity);
  }

  void LoreScene::startLoad()
  {
    m_clock.restart();
    m_result = std::async(std::launch::async, [&]() {
      m_game.state = generateNewGame(m_game.random, m_game.resources);
    });
  }

  bool LoreScene::isLoaded()
  {
    if (m_result.valid() && m_result.wait_for(std::chrono::seconds::zero()) == std::future_status::ready) {
      m_result.get();
      auto time = m_clock.getElapsedTime();
      gf::Log::debug("Async load in %i ms\n", time.asMilliseconds());
      return true;
    }

    return false;
  }

  void LoreScene::doUpdate([[maybe_unused]] gf::Time time)
  {
    if (!m_generation_finished && isLoaded()) {
      m_generation_finished = true;
      m_game.miniMap.initMap();
      addHudEntity(m_loreEndEntity);
    }
  }

  void LoreScene::doHandleActions([[maybe_unused]] gf::Window& window)
  {
    if (m_generation_finished && m_nextAction.isActive()) {
      m_game.replaceScene(m_game.contract);
    }
  }

}
