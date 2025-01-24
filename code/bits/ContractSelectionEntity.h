#ifndef BE_CONTRACT_SELECTION_H
#define BE_CONTRACT_SELECTION_H

#include "gf/Vector.h"
#include <gf/Entity.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>

namespace be {
  struct GameHub;

  class ContractSelectionEntity : public gf::Entity {
  public:
    ContractSelectionEntity(GameHub& game);

    void updateContracts();

    void pointTo(const gf::Vector2f& cursor);
    void triggerAction();

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameHub& m_game;
    gf::Font& m_font;

    gf::TextButtonWidget m_choice1;
    gf::TextButtonWidget m_choice2;
    gf::TextButtonWidget m_choice3;

    gf::WidgetContainer m_widgets;
  };

}

#endif // BE_CONTRACT_SELECTION_H