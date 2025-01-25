#include "ContractScene.h"

#include <cassert>
#include <cstddef>

#include <gf/Log.h>

#include "BubbleState.h"
#include "CityState.h"
#include "GameHub.h"
#include "GameState.h"

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

  void ContractScene::selectNextContract(int index)
  {
    assert(index >= 0 && static_cast<size_t>(index) < m_nextContracts.size());
    assert(m_nextContracts[index].targetCity >= 0 && m_nextContracts[index].targetCity < int(CityCount));
    assert(m_nextContracts[index].targetCity != m_game.state.contract.targetCity);
    m_game.state.contract = m_nextContracts[index];

    const auto& cityName = m_game.state.cities[m_game.state.contract.targetCity].name;
    gf::Log::debug("New target City: '%s'\n", cityName.c_str());

    m_game.replaceAllScenes(m_game.world);
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
    const auto& previousContract = m_game.state.contract;

    m_game.state.contractProgress = gf::clamp(m_game.state.contractProgress + ContractStep, ContractStep, ContractMaxStep);

    for (int i = 0; i < int(m_nextContracts.size()); ++i) {
      auto& nextContract = m_nextContracts[i];
      nextContract.originCity = previousContract.targetCity;
      nextContract.bubbleValueTarget = std::round(m_game.random.computeUniformFloat((m_game.state.contractProgress - 0.1f) * MaxContract, m_game.state.contractProgress * MaxContract));
      if (i < nextContract.originCity) {
        nextContract.targetCity = i;
      } else {
        nextContract.targetCity = i + 1;
      }
      int typeIndex = m_game.random.computeUniformInteger(static_cast<size_t>(0), CityCount - 1);
      if (typeIndex == nextContract.targetCity) {
        typeIndex = (typeIndex + 1) % CityCount;
      }
      nextContract.type = static_cast<BubbleType>(typeIndex);
      assert(nextContract.type != BubbleType::None);
    }

    m_selection.updateContracts(m_nextContracts);

    m_game.state.deleteAllBubbles();
  }
}
