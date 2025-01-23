#ifndef BE_CONTRACT_SCENE_H
#define BE_CONTRACT_SCENE_H

#include <gf/Font.h>
#include <gf/Scene.h>
#include <gf/Views.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>

namespace be {
  struct GameHub;

  class ContractScene : public gf::Scene {
  public:
    ContractScene(GameHub& game);

    void updateContracts();

  private:
    void doHandleActions(gf::Window& window) override;
    void doProcessEvent(gf::Event& event) override;
    void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;

    void goToWorld();

    GameHub& m_game;

    gf::Font& m_font;

    gf::Action m_triggerAction;

    gf::TextButtonWidget m_choice1;
    gf::TextButtonWidget m_choice2;
    gf::TextButtonWidget m_choice3;

    gf::WidgetContainer m_widgets;
  };

}

#endif // BE_CONTRACT_SCENE_H
