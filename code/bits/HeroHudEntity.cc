#include "HeroHudEntity.h"

#include <gf/Anchor.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/StringUtils.h>
#include <gf/AnimatedSprite.h>
#include <gf/Text.h>

#include "GameHub.h"
#include "GameState.h"

namespace be {
  HeroHudEntity::HeroHudEntity(GameHub& game)
  : m_state(game.state)
  , m_font(game.resources.getFont("DejaVuSans.ttf"))
  , m_redBubbleTexture(game.resources.getTexture("animation/bubble_red.png"))
  , m_blueBubbleTexture(game.resources.getTexture("animation/bubble_blue.png"))
  , m_greenBubbleTexture(game.resources.getTexture("animation/bubble_green.png"))
  , m_yellowBubbleTexture(game.resources.getTexture("animation/bubble_yellow.png"))
  {
    m_redBubbleAnimation.addTileset(m_redBubbleTexture, gf::vec(8, 8), gf::seconds(1.0f / 25.0f), 64);
    m_blueBubbleAnimation.addTileset(m_blueBubbleTexture, gf::vec(8, 8), gf::seconds(1.0f / 25.0f), 64);
    m_greenBubbleAnimation.addTileset(m_greenBubbleTexture, gf::vec(8, 8), gf::seconds(1.0f / 25.0f), 64);
    m_yellowBubbleAnimation.addTileset(m_yellowBubbleTexture, gf::vec(8, 8), gf::seconds(1.0f / 25.0f), 64);
  }

  void HeroHudEntity::update(gf::Time time) {
    switch (m_state.contract.type)
    {
    case BubbleType::Red:
      m_contractBubble = &m_redBubbleAnimation;
      break;
    
    case BubbleType::Blue:
      m_contractBubble = &m_blueBubbleAnimation;
      break;
    
    case BubbleType::Green:
      m_contractBubble = &m_greenBubbleAnimation;
      break;
    
    case BubbleType::Yellow:
      m_contractBubble = &m_yellowBubbleAnimation;
      break;
    
    default:
      assert(false);
      break;
    }
    m_contractBubble->update(time);
  }

  void HeroHudEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Coordinates coords(target);

    const float bubbleValue = m_state.computeBubblesValues();
    gf::Text text(gf::niceNum(bubbleValue, 1.0f) + " / " + gf::niceNum(m_state.contract.bubbleValueTarget, 1.0f), m_font);
    text.setColor(gf::Color::White);
    text.setAnchor(gf::Anchor::CenterLeft);
    text.setPosition(coords.getRelativePoint({0.1f, 0.1f}));
    target.draw(text, states);

    gf::AnimatedSprite contractBubble;
    contractBubble.setAnimation(*m_contractBubble);
    contractBubble.setAnchor(gf::Anchor::CenterLeft);
    contractBubble.setPosition(coords.getRelativePoint({0.1f, 0.1f}) - gf::Vector2f{70.0f, 5.0f});
    contractBubble.scale(HeroHudBubbleScale);
    target.draw(contractBubble, states);
  }

}
