#include "TrapEntity.h"

#include <gf/Anchor.h>
#include <gf/AnimatedSprite.h>
#include <gf/Color.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Time.h>
#include <gf/VectorOps.h>

#include "GameHub.h"
#include "HeroState.h"
#include "TrapState.h"

namespace be {

  TrapEntity::TrapEntity(GameHub& game)
  : m_state(game.state)
  , m_spawnTileset(game.resources.getTexture("animation/trap_spawn.png"))
  , m_triggerTileset(game.resources.getTexture("animation/trap_trigger.png"))
  , m_growTileset(game.resources.getTexture("animation/trap_grow.png"))
  , m_activeTileset(game.resources.getTexture("animation/trap_active.png"))
  , m_restTileset(game.resources.getTexture("animation/trap_rest.png"))
  {
    m_spawnTileset.setSmooth();
    m_spawnAnimation.addTileset(m_spawnTileset, gf::vec(4, 4), gf::seconds(1.0f / 25.0f), 13);
    m_spawnAnimation.setLoop(false);

    m_triggerTileset.setSmooth();
    m_triggerAnimation.addTileset(m_triggerTileset, gf::vec(8, 8), gf::seconds(1.0f / 25.0f), 24);

    m_growTileset.setSmooth();
    m_growAnimation.addTileset(m_growTileset, gf::vec(8, 8), gf::seconds(1.0f / 25.0f), 41);
    m_growAnimation.setLoop(false);

    m_activeTileset.setSmooth();
    m_activeAnimation.addTileset(m_activeTileset, gf::vec(8, 8), gf::seconds(1.0f / 25.0f), 18);

    m_restTileset.setSmooth();
    m_restAnimation.addTileset(m_restTileset, gf::vec(8, 8), gf::seconds(1.0f / 25.0f), 30);
    m_restAnimation.setLoop(false);
  }

  void TrapEntity::update(gf::Time time)
  {
    for (auto& trap: m_state.traps) {
      switch (trap.status) {
      case TrapStatus::Wait:
        if (gf::squareDistance(trap.spot.location, m_state.hero.location) <= (TrapTriggerDistance * TrapTriggerDistance)) {
          trap.clock.restart();
          m_spawnAnimation.reset();
          m_triggerAnimation.reset();
          trap.status = TrapStatus::Trigger;
        }
        break;

      case TrapStatus::Trigger:
        if (m_spawnAnimation.isFinished()) {
          m_triggerAnimation.update(time);
        } else {
          m_spawnAnimation.update(time);
        }
        if (trap.clock.getElapsedTime().asSeconds() >= TrapTriggerTime) {
          m_growAnimation.reset();
          m_activeAnimation.reset();
          trap.status = TrapStatus::Active;
        }
        break;

      case TrapStatus::Active:
        if (trap.clock.getElapsedTime().asSeconds() >= TrapActiveTime) {
          m_restAnimation.reset();
          trap.status = TrapStatus::Rest;
        } else {
          if (m_growAnimation.isFinished()) {
            m_activeAnimation.update(time);
          } else {
            m_growAnimation.update(time);
          }
          trap.radius = gf::clamp(TrapGrowRate * time.asSeconds() + trap.radius, 0.0f, TrapMaxHitRadius);
          if (gf::squareDistance(trap.spot.location, m_state.hero.location) <= (trap.radius * trap.radius)
            && m_state.hero.lastHitTime.asSeconds() >= HeroInvincibilityTime) {
            m_state.hero.lastHitTime = gf::Time::zero();
            --m_state.hero.life;
            gf::Log::debug("Player hit!\n");
          }
        }
        break;

      case TrapStatus::Rest:
        m_restAnimation.update(time);
        if (trap.clock.getElapsedTime().asSeconds() >= TrapRestTime) {
          trap.status = TrapStatus::Wait;
          trap.radius = 0.0f;
        }
        break;
      }
    }
  }

  void TrapEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    for (const auto& trap: m_state.traps) {
      switch (trap.status) {
      case TrapStatus::Wait:
        // {
        //   gf::RectangleShape rect({32.0f, 32.0f});
        //   rect.setColor(gf::Color::Magenta);
        //   rect.setAnchor(gf::Anchor::Center);
        //   rect.setPosition(trap.spot.location);
        //   target.draw(rect, states);
        // }
        break;

      case TrapStatus::Trigger:
        {
          gf::AnimatedSprite animation;
          if (m_spawnAnimation.isFinished()) {
            animation.setAnimation(m_triggerAnimation);
          } else {
            animation.setAnimation(m_spawnAnimation);
          }
          animation.setAnchor(gf::Anchor::Center);
          animation.setPosition(trap.spot.location);
          animation.setScale(0.5f);
          target.draw(animation, states);
        }
        break;

      case TrapStatus::Active:
        {
          gf::AnimatedSprite animation;
          if (m_growAnimation.isFinished()) {
            animation.setAnimation(m_activeAnimation);
          } else {
            animation.setAnimation(m_growAnimation);
          }
          animation.setAnchor(gf::Anchor::Center);
          animation.setPosition(trap.spot.location);
          animation.setScale(0.5f);
          target.draw(animation, states);
        }
        break;

      case TrapStatus::Rest:
        {
          gf::AnimatedSprite animation;
          animation.setAnimation(m_restAnimation);
          animation.setAnchor(gf::Anchor::Center);
          animation.setPosition(trap.spot.location);
          animation.setScale(0.5f);
          target.draw(animation, states);
        }
        break;
      }
    }
  }

}
