#include "HeroHintEntity.h"

#include <gf/RenderTarget.h>
#include <gf/Math.h>
#include <gf/Sprite.h>

#include "CityState.h"
#include "GameHub.h"

namespace be {

  namespace {

    constexpr float HintMinDistance = 1000.0f;
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

      gf::Vector2f rawDirectionCity = m_state.cities[m_state.contract.targetCity].spot.location - m_state.hero.location;

      if (gf::squareLength(rawDirectionCity) > gf::square(HintMinDistance)) {
        gf::Vector2f directionCity = gf::normalize(rawDirectionCity);
        gf::Sprite targetCityArrow(m_arrowTexture);
        targetCityArrow.setColor(gf::Color::Yellow);
        targetCityArrow.setAnchor(gf::Anchor::Center);
        targetCityArrow.setPosition(m_state.hero.location + ArrowRange * directionCity);
        targetCityArrow.setRotation(gf::angle(directionCity));
        targetCityArrow.setScale(ArrowScale);
        target.draw(targetCityArrow, states);
      }

    } else {
      // show city direction

      auto iteratorBubble = std::find_if(m_state.cities.begin(), m_state.cities.end(), [&](const CityState& city) {
        return city.type == m_state.contract.type;
      });

      assert(iteratorBubble != m_state.cities.end());

      gf::Vector2f directionBubble = gf::normalize(iteratorBubble->center - m_state.hero.location);

      gf::Sprite targetBubbleArrow(m_arrowTexture);
      targetBubbleArrow.setColor(gf::Color::Yellow);
      targetBubbleArrow.setAnchor(gf::Anchor::Center);
      targetBubbleArrow.setPosition(m_state.hero.location + ArrowRange * directionBubble);
      targetBubbleArrow.setRotation(gf::angle(directionBubble));
      targetBubbleArrow.setScale(ArrowScale);
      target.draw(targetBubbleArrow, states);
    }

  }

}
