#include "HeroHintEntity.h"

#include <gf/RenderTarget.h>
#include <gf/Math.h>
#include <gf/Sprite.h>

#include "CityState.h"
#include "GameHub.h"
#include "Support.h"

namespace be {

  namespace {

    constexpr float HintMinDistance = 1000.0f;
    constexpr float HintMaxDistance = 1200.0f;
    constexpr float ArrowRange = 80.0f;
    constexpr float ArrowScale = 0.05f;

  }

  HeroHintEntity::HeroHintEntity(GameHub& game)
  : m_state(game.state)
  , m_arrowTexture(game.resources.getTexture("caret-right-solid.png"))
  {
  }

  void HeroHintEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    const auto bubbleValues = m_state.computeBubblesValues();

    if (bubbleValues < m_state.contract.bubbleValueTarget) {
      // show bubble directions

      auto iteratorBubble = std::find_if(m_state.cities.begin(), m_state.cities.end(), [&](const CityState& city) {
        return city.type == m_state.contract.type;
      });

      assert(iteratorBubble != m_state.cities.end());

      gf::Vector2f rawDirectionBubble = iteratorBubble->center - m_state.hero.location;
      const float bubbleDistance = gf::euclideanLength(rawDirectionBubble);

      if (bubbleDistance > HintMinDistance) {
        gf::Vector2f directionBubble = rawDirectionBubble / bubbleDistance;

        gf::Sprite targetBubbleArrow(m_arrowTexture);
        targetBubbleArrow.setColor(gf::Color::Yellow * gf::Color::Opaque(smooth(bubbleDistance, HintMinDistance, HintMaxDistance)));
        targetBubbleArrow.setAnchor(gf::Anchor::Center);
        targetBubbleArrow.setPosition(m_state.hero.location + ArrowRange * directionBubble);
        targetBubbleArrow.setRotation(gf::angle(directionBubble));
        targetBubbleArrow.setScale(ArrowScale);
        target.draw(targetBubbleArrow, states);
      }

    } else {
      // show city direction

      gf::Vector2f rawDirectionCity = m_state.cities[m_state.contract.targetCity].spot.location - m_state.hero.location;

      gf::Vector2f directionCity = gf::normalize(rawDirectionCity);
      gf::Sprite targetCityArrow(m_arrowTexture);
      targetCityArrow.setColor(gf::Color::Yellow);
      targetCityArrow.setAnchor(gf::Anchor::Center);
      targetCityArrow.setPosition(m_state.hero.location + ArrowRange * directionCity);
      targetCityArrow.setRotation(gf::angle(directionCity));
      targetCityArrow.setScale(ArrowScale);
      target.draw(targetCityArrow, states);

    }

  }

}
