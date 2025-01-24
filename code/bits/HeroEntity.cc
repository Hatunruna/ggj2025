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

    gf::Vector2f direction = gf::normalize(m_state.cities[m_state.contract.targetCity].location - m_state.hero.location);
    gf::CircleShape circle(10, 3);
    circle.setColor(gf::Color::Red);
    circle.setAnchor(gf::Anchor::Center);
    circle.setPosition(m_state.hero.location + 128.0f * direction);
    circle.setRotation(gf::angle(direction) + gf::Pi2);
    target.draw(circle, states);
  }

}
