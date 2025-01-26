#include "HeroEntity.h"

#include <gf/AnimatedSprite.h>
#include <gf/RenderTarget.h>
#include <gf/Math.h>
#include <gf/Sprite.h>

#include "CityState.h"
#include "GameHub.h"

namespace be {

  HeroEntity::HeroEntity(GameHub& game)
  : m_state(game.state)
  , m_runUpTexture(game.resources.getTexture("animation/run_up.png"))
  , m_runDownTexture(game.resources.getTexture("animation/run_down.png"))
  , m_runLeftTexture(game.resources.getTexture("animation/run_left.png"))
  , m_runUpLeftTexture(game.resources.getTexture("animation/run_up_left.png"))
  , m_runDownRightTexture(game.resources.getTexture("animation/run_down_right.png"))
  , m_idleTexture(game.resources.getTexture("animation/idle.png"))
  {
    m_runUpAnimation.addTileset(m_runUpTexture, gf::vec(4, 3), gf::seconds(1.0f / 25.0f), 12);
    m_runDownAnimation.addTileset(m_runDownTexture, gf::vec(4, 3), gf::seconds(1.0f / 25.0f), 12);
    m_runLeftAnimation.addTileset(m_runLeftTexture, gf::vec(4, 3), gf::seconds(1.0f / 25.0f), 12);
    m_runUpLeftAnimation.addTileset(m_runUpLeftTexture, gf::vec(4, 3), gf::seconds(1.0f / 25.0f), 12);
    m_runDownRightAnimation.addTileset(m_runDownRightTexture, gf::vec(4, 3), gf::seconds(1.0f / 25.0f), 12);
    m_idleAnimation.addTileset(m_idleTexture, gf::vec(4, 3), gf::seconds(1.0f / 25.0f), 12);
  }

  void HeroEntity::update(gf::Time time) {
    m_state.hero.orientation = gf::Orientation::Center;
    if (!(cpBodyGetVelocity(m_state.hero.control) == cpvzero)) {
      m_state.hero.orientation = gf::orientation(cpBodyGetAngle(m_state.hero.control) + gf::Pi);
    }

    switch (m_state.hero.orientation) {
    case gf::Orientation::Center:
      m_currentAnimation = &m_idleAnimation;
      break;

    case gf::Orientation::South:
      m_currentAnimation = &m_runUpAnimation;
      break;

    case gf::Orientation::SouthEast:
      m_currentAnimation = &m_runUpLeftAnimation;
      break;

    case gf::Orientation::East:
      m_currentAnimation = &m_runLeftAnimation;

      break;

    case gf::Orientation::NorthEast:
      m_currentAnimation = &m_runDownRightAnimation;
      break;

    case gf::Orientation::North:
      m_currentAnimation = &m_runDownAnimation;
      break;

    case gf::Orientation::NorthWest:
      m_currentAnimation = &m_runDownRightAnimation;
      break;

    case gf::Orientation::West:
      m_currentAnimation = &m_runLeftAnimation;
      break;

    case gf::Orientation::SouthWest:
      m_currentAnimation = &m_runUpLeftAnimation;
      break;
    }

    m_currentAnimation->update(time);
  }

  void HeroEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Vector2f directionCity = gf::normalize(m_state.cities[m_state.contract.targetCity].spot.location - m_state.hero.location);
    gf::CircleShape targetCityArrow(ArrowSize, 3);
    targetCityArrow.setColor(gf::Color::Red);
    targetCityArrow.setAnchor(gf::Anchor::Center);
    targetCityArrow.setPosition(m_state.hero.location + ArrowRange * directionCity);
    targetCityArrow.setRotation(gf::angle(directionCity) + gf::Pi2);
    target.draw(targetCityArrow, states);

    auto iteratorBubble = std::find_if(m_state.cities.begin(), m_state.cities.end(), [&](const CityState& city) {
      return city.type == m_state.contract.type;
    });

    assert(iteratorBubble != m_state.cities.end());

    gf::Vector2f directionBubble = gf::normalize(iteratorBubble->center - m_state.hero.location);
    gf::CircleShape targetBubbleArrow(ArrowSize, 3);
    targetBubbleArrow.setColor(gf::Color::Yellow);
    targetBubbleArrow.setAnchor(gf::Anchor::Center);
    targetBubbleArrow.setPosition(m_state.hero.location + ArrowRange * directionBubble);
    targetBubbleArrow.setRotation(gf::angle(directionBubble) + gf::Pi2);
    target.draw(targetBubbleArrow, states);

    gf::AnimatedSprite animatedSprite;
    animatedSprite.setAnimation(*m_currentAnimation);
    animatedSprite.setPosition(m_state.hero.location);
    animatedSprite.setAnchor(gf::Anchor::Center);
    animatedSprite.setScale(HeroScale);
    if (m_state.hero.orientation==gf::Orientation::West
      || m_state.hero.orientation==gf::Orientation::SouthWest
      || m_state.hero.orientation==gf::Orientation::NorthEast)
    {
      animatedSprite.setScale(HeroScale * gf::Vector2f{-1.0f, 1.0f});
    }
    target.draw(animatedSprite, states);
  }

}
