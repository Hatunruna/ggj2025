#include "BubbleEntity.h"

#include <cassert>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

#include "GameHub.h"

namespace be {

  BubbleEntity::BubbleEntity(GameHub& game)
  : m_state(game.state)
  , m_producerTexture(game.resources.getTexture("producers.png"))
  , m_redBubbleTexture(game.resources.getTexture("redBubble.png"))
  , m_blueBubbleTexture(game.resources.getTexture("blueBubble.png"))
  , m_greenBubbleTexture(game.resources.getTexture("greenBubble.png"))
  , m_yellowBubbleTexture(game.resources.getTexture("yellowBubble.png"))
  {
  }

  void BubbleEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Sprite producerSprite(m_producerTexture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f / 2.0f, 1.0f / 2.0f }));
    producerSprite.setAnchor(gf::Anchor::TopCenter);

    gf::Sprite bubbleSprite;

    auto drawBubble = [&](const gf::Vector2f& position, float size, BubbleType type, gf::Anchor anchor) -> void {
      switch (type) {
      case BubbleType::Red:
        bubbleSprite.setTexture(m_redBubbleTexture);
        break;
      case BubbleType::Green:
        bubbleSprite.setTexture(m_greenBubbleTexture);
        break;
      case BubbleType::Blue:
        bubbleSprite.setTexture(m_blueBubbleTexture);
        break;
      case BubbleType::Yellow:
        bubbleSprite.setTexture(m_yellowBubbleTexture);
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
      producerSprite.setPosition(producer.location);
      target.draw(producerSprite, states);

      drawBubble(producer.location, producer.size, producer.type, gf::Anchor::BottomCenter);
    }

    bubbleSprite.setAnchor(gf::Anchor::Center);

    for (auto& bubble : m_state.bubbles) {
      cpVect location = cpBodyGetPosition(bubble.body);

      drawBubble(gf::vec(location.x, location.y), bubble.size, bubble.type, gf::Anchor::Center);
    }

    for (auto& bubble : m_state.free_bubbles) {
      cpVect location = cpBodyGetPosition(bubble.body);

      drawBubble(gf::vec(location.x, location.y), bubble.size, bubble.type, gf::Anchor::Center);
    }

  }

}
