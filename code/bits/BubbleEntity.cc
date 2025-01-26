#include "BubbleEntity.h"

#include <cassert>
#include <gf/AnimatedSprite.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

#include "BubbleState.h"
#include "GameHub.h"

namespace be {

  BubbleEntity::BubbleEntity(GameHub& game)
  : m_state(game.state)
  , m_producerTexture(game.resources.getTexture("producers.png"))
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

  void BubbleEntity::update(gf::Time time) {
    m_redBubbleAnimation.update(time);
    m_blueBubbleAnimation.update(time);
    m_greenBubbleAnimation.update(time);
    m_yellowBubbleAnimation.update(time);
  }

  void BubbleEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::AnimatedSprite bubbleSprite;

    auto drawBubble = [&](const gf::Vector2f& position, float size, BubbleType type, gf::Anchor anchor) -> void {
      switch (type) {
      case BubbleType::Red:
        bubbleSprite.setAnimation(m_redBubbleAnimation);
        break;
      case BubbleType::Green:
        bubbleSprite.setAnimation(m_greenBubbleAnimation);
        break;
      case BubbleType::Blue:
        bubbleSprite.setAnimation(m_blueBubbleAnimation);
        break;
      case BubbleType::Yellow:
        bubbleSprite.setAnimation(m_yellowBubbleAnimation);
        break;

      case BubbleType::None:
        assert(false);
      }

      bubbleSprite.setAnchor(anchor);
      bubbleSprite.setPosition(position);
      bubbleSprite.setScale(size);
      target.draw(bubbleSprite, states);
    };

    for (auto& producer : m_state.producers) {
      gf::Sprite producerSprite(m_producerTexture, gf::RectF::fromPositionSize({ 1.0f / 3.0f * producer.tile, 0.0f }, { 1.0f / 3.0f, 1.0f }));
      producerSprite.setAnchor(gf::Anchor::TopCenter);
      producerSprite.setPosition(producer.spot.location);
      producerSprite.setScale(0.4f);
      target.draw(producerSprite, states);

      if (producer.status == BubbleProducerStatus::Emerging) {
        continue;
      }

      drawBubble(producer.spot.location, producer.size, producer.type, gf::Anchor::BottomCenter);
    }

    for (auto& bubble : m_state.bubbles) {
      cpVect location = cpBodyGetPosition(bubble.body);

      drawBubble(gf::vec(location.x, location.y), bubble.size, bubble.type, gf::Anchor::Center);
    }

    for (auto& bubble : m_state.freeBubbles) {
      cpVect location = cpBodyGetPosition(bubble.body);

      drawBubble(gf::vec(location.x, location.y), bubble.size, bubble.type, gf::Anchor::Center);
    }

  }

}
