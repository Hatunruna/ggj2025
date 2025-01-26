#include "TrapEntity.h"

#include <gf/Anchor.h>
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
  {

  }

  void TrapEntity::update(gf::Time time)
  {
    for (auto& trap: m_state.traps) {
      switch (trap.status) {
      case TrapStatus::Wait:
        if (gf::squareDistance(trap.spot.location, m_state.hero.location) <= (TrapTriggerDistance * TrapTriggerDistance)) {
          trap.clock.restart();
          trap.status = TrapStatus::Trigger;
        }
        break;

      case TrapStatus::Trigger:
        if (trap.clock.getElapsedTime().asSeconds() >= TrapTriggerTime) {
          trap.status = TrapStatus::Active;
        }
        break;

      case TrapStatus::Active:
        if (trap.clock.getElapsedTime().asSeconds() >= TrapActiveTime) {
          trap.status = TrapStatus::Rest;
        } else {
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
        {
          gf::RectangleShape rect({32.0f, 32.0f});
          rect.setColor(gf::Color::Magenta);
          rect.setAnchor(gf::Anchor::Center);
          rect.setPosition(trap.spot.location);
          target.draw(rect, states);
        }
        break;

      case TrapStatus::Trigger:
        {
          const auto elapsedTime = trap.clock.getElapsedTime().asMilliseconds();
          if ((elapsedTime / 500) % 2 == 1) {
            gf::RectangleShape rect({32.0f, 32.0f});
            rect.setColor(gf::Color::Yellow);
            rect.setAnchor(gf::Anchor::Center);
            rect.setPosition(trap.spot.location);
            target.draw(rect, states);
          }
        }
        break;

      case TrapStatus::Active:
        {
          gf::CircleShape circle(trap.radius);
          circle.setColor(gf::Color::Green);
          circle.setAnchor(gf::Anchor::Center);
          circle.setPosition(trap.spot.location);
          target.draw(circle, states);
        }
        break;

      case TrapStatus::Rest:
        {
          gf::RectangleShape rect({32.0f, 32.0f});
          rect.setColor(gf::Color::Black);
          rect.setAnchor(gf::Anchor::Center);
          rect.setPosition(trap.spot.location);
          target.draw(rect, states);
        }
        break;
      }
    }
  }

}
