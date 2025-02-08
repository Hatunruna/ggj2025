#include "HeroHudEntity.h"

#include <string>

#include <gf/Anchor.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/StringUtils.h>
#include <gf/AnimatedSprite.h>
#include <gf/Text.h>

#include "GameHub.h"
#include "GameState.h"
#include "HeroState.h"

namespace be {
  HeroHudEntity::HeroHudEntity(GameHub& game)
  : m_state(game.state)
  , m_font(game.resources.getFont("DejaVuSans.ttf"))
  , m_redBubbleTexture(game.resources.getTexture("animation/bubble_red.png"))
  , m_blueBubbleTexture(game.resources.getTexture("animation/bubble_blue.png"))
  , m_greenBubbleTexture(game.resources.getTexture("animation/bubble_green.png"))
  , m_yellowBubbleTexture(game.resources.getTexture("animation/bubble_yellow.png"))
  , m_heartTexture(game.resources.getTexture("heart-solid.png"))
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

    const gf::Vector2f scale = coords.getRelativeSize({ 1.f, 1.0f }) / gf::vec(1600.0f, 900.0f);

    constexpr float characterSize = 0.04f;
    const unsigned relativeCharacterSize = coords.getRelativeCharacterSize(characterSize);

    const float bubbleValue = m_state.computeBubblesValues();
    gf::Text bubbleText(gf::niceNum(bubbleValue, 1.0f) + " / " + gf::niceNum(m_state.contract.bubbleValueTarget, 1.0f), m_font, relativeCharacterSize);
    bubbleText.setColor(gf::Color::White);
    bubbleText.setAnchor(gf::Anchor::CenterLeft);
    bubbleText.setPosition(coords.getRelativePoint({0.11f, 0.1f}));
    target.draw(bubbleText, states);

    gf::AnimatedSprite contractBubble;
    contractBubble.setAnimation(*m_contractBubble);
    contractBubble.setAnchor(gf::Anchor::Center);
    contractBubble.setPosition(coords.getRelativePoint({0.05f, 0.1f}));
    contractBubble.scale(HeroHudBubbleScale * scale);
    target.draw(contractBubble, states);

    gf::Text healthText("HP", m_font, relativeCharacterSize);
    // text.setString("HP " + std::to_string(m_state.hero.life) + " / " + std::to_string(HeroMaxHealth));

    healthText.setAnchor(gf::Anchor::Center);
    healthText.setPosition(coords.getRelativePoint({0.75f, 0.1f}));
    target.draw(healthText, states);

    gf::Sprite healthSprite(m_heartTexture);
    healthSprite.setAnchor(gf::Anchor::Center);
    healthSprite.setScale(0.1f * scale);

    for (int i = 0; i < HeroMaxHealth; ++i) {
      healthSprite.setColor(i <= m_state.hero.life ? gf::Color::Red : gf::Color::Black);
      healthSprite.setPosition(coords.getRelativePoint({ 0.75f + 0.04f * i, 0.1f }));
      target.draw(healthSprite, states);
    }

  }

}
