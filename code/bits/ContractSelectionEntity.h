#ifndef BE_CONTRACT_SELECTION_ENTITY_H
#define BE_CONTRACT_SELECTION_ENTITY_H

#include <array>

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/Text.h>
#include <gf/Texture.h>
#include <gf/Vector.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>

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

    void update(gf::Time time) override;
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameHub& m_game;
    gf::Font& m_font;

    std::array<gf::Ref<gf::Texture>, CityCount> m_textures;
    gf::Animation* m_choicesBubble[3];

    gf::Animation m_redBubbleAnimation;
    gf::Animation m_blueBubbleAnimation;
    gf::Animation m_greenBubbleAnimation;
    gf::Animation m_yellowBubbleAnimation;
    std::array<gf::Ref<gf::Animation>, CityCount> m_animations;

    gf::Text m_text1;
    gf::Text m_text2;
    gf::Text m_text3;

    gf::TextButtonWidget m_choice1;
    gf::TextButtonWidget m_choice2;
    gf::TextButtonWidget m_choice3;

    gf::WidgetContainer m_widgets;
  };

}

#endif // BE_CONTRACT_SELECTION_ENTITY_H
