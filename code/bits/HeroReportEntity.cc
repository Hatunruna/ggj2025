#include "HeroReportEntity.h"

#include <gf/Coordinates.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

#include "GameHub.h"

namespace be {

  HeroReportEntity::HeroReportEntity(GameHub& game)
  : m_state(game.state)
  , m_font(game.resources.getFont("DejaVuSans.ttf"))
  , m_backgroundTexture(game.resources.getTexture("ui_background.jpg"))
  {
    m_backgroundTexture.setSmooth();
  }

  void HeroReportEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Coordinates coords(target);

    float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
    float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;

    gf::Sprite background(m_backgroundTexture);
    background.setPosition(coords.getCenter());
    background.setAnchor(gf::Anchor::Center);
    background.setScale(backgroundScale);
    target.draw(background, states);

    constexpr float characterSize = 0.055f;
    const unsigned relativeCharacterSize = coords.getRelativeCharacterSize(characterSize);

    constexpr gf::Vector2f ButtonStartPoint = gf::vec(0.75f, 0.25f);

    gf::Text currentCity("You are at " + m_state.cities[m_state.contract.targetCity].name, m_font, relativeCharacterSize * 0.90);
    currentCity.setAnchor(gf::Anchor::Center);
    currentCity.setColor(gf::Color::White);
    currentCity.setPosition(coords.getRelativePoint(gf::vec(0.25f, ButtonStartPoint.y - 0.1)));
    target.draw(currentCity, states);
  }

}
