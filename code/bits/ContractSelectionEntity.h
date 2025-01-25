#ifndef BE_CONTRACT_SELECTION_ENTITY_H
#define BE_CONTRACT_SELECTION_ENTITY_H

#include "gf/Vector.h"
#include <gf/Entity.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Text.h>

#include "CityState.h"
#include "ContractState.h"

namespace be {
  struct GameHub;

  class ContractSelectionEntity : public gf::Entity {
  public:
    ContractSelectionEntity(GameHub& game);

    void updateContracts(const std::array<ContractState, CityCount - 1>& nextContracts);

    void pointTo(const gf::Vector2f& cursor);
    void triggerAction();

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    gf::Texture& getContractBubbleTexture(BubbleType type);

    GameHub& m_game;
    gf::Font& m_font;

    gf::Text m_text1;
    gf::Text m_text2;
    gf::Text m_text3;

    gf::Texture& m_redBubble;
    gf::Texture& m_blueBubble;
    gf::Texture& m_greenBubble;
    gf::Texture& m_yellowBubble;
    gf::Texture* m_choicesBubble[3];

    gf::TextButtonWidget m_choice1;
    gf::TextButtonWidget m_choice2;
    gf::TextButtonWidget m_choice3;

    gf::WidgetContainer m_widgets;
  };

}

#endif // BE_CONTRACT_SELECTION_ENTITY_H
