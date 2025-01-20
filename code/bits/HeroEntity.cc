#include "HeroEntity.h"

#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

#include "GameHub.h"

namespace be {

  HeroEntity::HeroEntity(GameHub& game)
  : m_state(game.state)
  , m_texture(game.resources.getTexture("hero.png"))
  {
  }

  void HeroEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Sprite sprite(m_texture, gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 1.0f / 8.0f, 1.0f / 8.0f }));
    sprite.setPosition(m_state.hero.location);
    sprite.setAnchor(gf::Anchor::Center);
    target.draw(sprite, states);
  }

}
