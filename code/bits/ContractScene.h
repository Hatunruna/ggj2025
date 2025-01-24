#ifndef BE_CONTRACT_SCENE_H
#define BE_CONTRACT_SCENE_H

#include <gf/Scene.h>

#include "ContractSelectionEntity.h"
#include "HeroReportEntity.h"

namespace be {
  struct GameHub;

  class ContractScene : public gf::Scene {
  public:
    ContractScene(GameHub& game);

  private:
    void doHandleActions(gf::Window& window) override;
    void doProcessEvent(gf::Event& event) override;
    void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;

    void doShow() override;

    GameHub& m_game;

    gf::Action m_triggerAction;

    HeroReportEntity m_report;
    ContractSelectionEntity m_selection;
  };

}

#endif // BE_CONTRACT_SCENE_H
