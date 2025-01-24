#include "ContractScene.h"

#include "GameHub.h"

namespace be {

  ContractScene::ContractScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_triggerAction("TriggerAction")
  , m_report(game)
  , m_selection(game)
  {
    setClearColor(gf::Color::Black);

    m_triggerAction.addMouseButtonControl(gf::MouseButton::Left);
    addAction(m_triggerAction);

    addHudEntity(m_report);
    addHudEntity(m_selection);
  }

  void ContractScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
      return;
    }

    if (m_triggerAction.isActive()) {
      m_selection.triggerAction();
    }
  }

  void ContractScene::doProcessEvent(gf::Event& event) {
    switch (event.type)
    {
      case gf::EventType::MouseMoved:
        m_selection.pointTo(m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
        break;

      default:
        break;
    }
  }

  void ContractScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    renderHudEntities(target, states);
  }

  void ContractScene::doShow()
  {
    m_selection.updateContracts();
  }
}
