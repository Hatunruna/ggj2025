#include "HeroReportEntity.h"

#include <gf/Coordinates.h>
#include <gf/Text.h>

#include "GameHub.h"

namespace be {

  HeroReportEntity::HeroReportEntity(GameHub& game)
  : m_state(game.state)
  , m_font(game.resources.getFont("DejaVuSans.ttf"))
  {

  }

  void HeroReportEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Coordinates coords(target);

    constexpr float characterSize = 0.055f;
    const unsigned relativeCharacterSize = coords.getRelativeCharacterSize(characterSize);

    constexpr gf::Vector2f ButtonStartPoint = gf::vec(0.75f, 0.25f);
    constexpr float spaceBetweenButton = 0.10f;
    constexpr gf::Vector2f backgroundSize(0.35f, 0.40f);

    gf::Text currentCity("You are at " + m_state.cities[m_state.contract.originCity].name, m_font, relativeCharacterSize * 0.90);
    currentCity.setAnchor(gf::Anchor::Center);
    currentCity.setColor(gf::Color::White);
    currentCity.setPosition(coords.getRelativePoint(gf::vec(0.25f, ButtonStartPoint.y)));
    target.draw(currentCity, states);
  }

}