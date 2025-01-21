#include "BubbleEntity.h"

#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

#include "GameHub.h"

namespace be {

  BubbleEntity::BubbleEntity(GameHub& game)
  : m_state(game.state)
  , m_producerTexture(game.resources.getTexture("producers.png"))
  , m_bubbleTexture(game.resources.getTexture("bubble.png"))
  {
  }

  void BubbleEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Sprite producerSprite(m_producerTexture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f / 2.0f, 1.0f / 2.0f }));
    producerSprite.setAnchor(gf::Anchor::TopCenter);

    gf::Sprite bubbleSprite(m_bubbleTexture);
    bubbleSprite.setAnchor(gf::Anchor::BottomCenter);

    for (auto& producer : m_state.producers) {
      producerSprite.setPosition(producer.location);
      target.draw(producerSprite, states);

      bubbleSprite.setPosition(producer.location);
      bubbleSprite.setScale(producer.size);
      target.draw(bubbleSprite, states);
    }

    bubbleSprite.setAnchor(gf::Anchor::Center);

    for (auto& bubble : m_state.bubbles) {
      cpVect location = cpBodyGetPosition(bubble.body);

      bubbleSprite.setPosition(gf::vec(location.x, location.y));
      bubbleSprite.setScale(bubble.size);
      target.draw(bubbleSprite, states);
    }

  }

}
