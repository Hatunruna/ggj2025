#include "WorldScene.h"

#include "GameHub.h"
#include "HeroState.h"
#include "gf/Gamepad.h"

namespace be {

  WorldScene::WorldScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_upAction("up")
  , m_leftAction("left")
  , m_downAction("down")
  , m_rightAction("right")
  , m_takeAction("take")
  , m_releaseAction("release")
  , m_miniMapAction("minmap")
  , m_debugAction("debug")
  , m_mapEntity(game)
  , m_trapEntity(game)
  , m_heroEntity(game)
  , m_heroHintEntity(game)
  , m_bubbleEntity(game)
  , m_cityEntity(game)
  , m_mountainsEntity(game)
  , m_gateEntity(game)
  , m_heroHudEntity(game)
  , m_feedbackEntity(game)
  , m_physicsDebug(game)
  , m_adaptor(game.getRenderer(), getWorldView())
  , m_ambianceMusic(game.audio.getMusic("sounds/stage.ogg"))
  {
    setClearColor(gf::Color::Black);

    m_upAction.addScancodeKeyControl(gf::Scancode::Up);
    m_upAction.addScancodeKeyControl(gf::Scancode::W);
    m_upAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Negative);
    m_upAction.setContinuous();
    addAction(m_upAction);

    m_leftAction.addScancodeKeyControl(gf::Scancode::Left);
    m_leftAction.addScancodeKeyControl(gf::Scancode::A);
    m_leftAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Negative);
    m_leftAction.setContinuous();
    addAction(m_leftAction);

    m_downAction.addScancodeKeyControl(gf::Scancode::Down);
    m_downAction.addScancodeKeyControl(gf::Scancode::S);
    m_downAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Positive);
    m_downAction.setContinuous();
    addAction(m_downAction);

    m_rightAction.addScancodeKeyControl(gf::Scancode::Right);
    m_rightAction.addScancodeKeyControl(gf::Scancode::D);
    m_rightAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Positive);
    m_rightAction.setContinuous();
    addAction(m_rightAction);

    m_takeAction.addScancodeKeyControl(gf::Scancode::Space);
    m_takeAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
    addAction(m_takeAction);

    m_releaseAction.addScancodeKeyControl(gf::Scancode::RightShift);
    m_releaseAction.addScancodeKeyControl(gf::Scancode::E);
    m_releaseAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::B);
    addAction(m_releaseAction);

    m_miniMapAction.addKeycodeKeyControl(gf::Keycode::M);
    m_miniMapAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Start);
    addAction(m_miniMapAction);

    m_debugAction.addScancodeKeyControl(gf::Scancode::F12);
    addAction(m_debugAction);

    addModel(game.state);

    addWorldEntity(m_mapEntity);
    addWorldEntity(m_trapEntity);
    addWorldEntity(m_heroEntity);
    addWorldEntity(m_bubbleEntity);
    addWorldEntity(m_cityEntity);
    addWorldEntity(m_mountainsEntity);
    addWorldEntity(m_gateEntity);
    addWorldEntity(m_heroHintEntity);

    addWorldEntity(m_physicsDebug);

    setWorldViewSize({ 512, 512 });
    setWorldViewCenter(game.state.hero.location);

    addHudEntity(m_heroHudEntity);
    addHudEntity(m_feedbackEntity);

    m_ambianceMusic.setLoop(true);
    m_ambianceMusic.play();
  }

  void WorldScene::doProcessEvent(gf::Event& event)
  {
    m_adaptor.processEvent(event);

    if (event.type == gf::EventType::MouseButtonReleased) {
      if (event.mouseButton.button == gf::MouseButton::Middle) {
        m_game.state.teleportHero(m_game.computeWindowToGameCoordinates(event.mouseButton.coords, getWorldView()));
      }
    }
  }

  void WorldScene::doHandleActions([[maybe_unused]] gf::Window& window)
  {
    if (m_miniMapAction.isActive()) {
      m_game.replaceScene(m_game.miniMap);
    }

    gf::Vector2i direction = { 0, 0 };

    if (m_upAction.isActive()) {
      direction.y = -1;
    } else if (m_downAction.isActive()) {
      direction.y = +1;
    }

    if (m_leftAction.isActive()) {
      direction.x = -1;
    } else if (m_rightAction.isActive()) {
      direction.x = +1;
    }

    m_game.state.moveHero(direction);

    if (m_takeAction.isActive()) {
      if (m_game.state.tryToEnterCity()) {
        m_game.replaceScene(m_game.contract);
      } else {
        m_game.state.tryToTakeBubble();
      }
    }

    if (m_releaseAction.isActive()) {
      m_game.state.releaseBubble();
    }

    if (m_debugAction.isActive()) {
      m_physicsDebug.toggleDebug();
    }

  }

  void WorldScene::doUpdate([[maybe_unused]] gf::Time time)
  {
    if (m_game.state.hero.life <= 0) {
      m_game.replaceScene(m_game.start);
    }
  }

  void WorldScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    setWorldViewCenter(m_game.state.hero.location);
    renderWorldEntities(target, states);
    renderHudEntities(target, states);
  }

  void WorldScene::doShow()
  {
    m_mapEntity.updateTiles();
  }

}
