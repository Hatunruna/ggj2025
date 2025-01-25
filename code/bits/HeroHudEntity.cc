#include "HeroHudEntity.h"

#include <gf/Anchor.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/StringUtils.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

#include "GameHub.h"
#include "GameState.h"

namespace be {
  HeroHudEntity::HeroHudEntity(GameHub& game)
  : m_state(game.state)
  , m_font(game.resources.getFont("DejaVuSans.ttf"))
  , m_redBubble(game.resources.getTexture("redBubble.png"))
  , m_blueBubble(game.resources.getTexture("blueBubble.png"))
  , m_greenBubble(game.resources.getTexture("greenBubble.png"))
  , m_yellowBubble(game.resources.getTexture("yellowBubble.png"))
  {
  }

  void HeroHudEntity::setContractBubbleTexture() {
    switch (m_state.contract.type)
    {
    case BubbleType::Red:
      m_contractBubble = &m_redBubble;
      break;
    
    case BubbleType::Blue:
      m_contractBubble = &m_blueBubble;
      break;
    
    case BubbleType::Green:
      m_contractBubble = &m_greenBubble;
      break;
    
    case BubbleType::Yellow:
      m_contractBubble = &m_yellowBubble;
      break;
    
    default:
      assert(false);
      break;
    }
  }

  void HeroHudEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Coordinates coords(target);

    const float bubbleValue = m_state.computeBubblesValues();
    gf::Text text("Cargo: " + gf::niceNum(bubbleValue, 1.0f) + " / " + gf::niceNum(m_state.contract.bubbleValueTarget, 1.0f), m_font);
    text.setColor(gf::Color::White);
    text.setAnchor(gf::Anchor::CenterLeft);
    text.setPosition(coords.getRelativePoint({0.1f, 0.1f}));
    target.draw(text, states);

    setContractBubbleTexture();

    gf::Sprite contractBubble(*m_contractBubble);
    contractBubble.setAnchor(gf::Anchor::CenterLeft);
    contractBubble.setPosition(coords.getRelativePoint({0.1f, 0.1f}) - gf::Vector2f{60.0f, 5.0f});
    contractBubble.scale(HeroHudBubbleScale);
    target.draw(contractBubble, states);
  }

}
