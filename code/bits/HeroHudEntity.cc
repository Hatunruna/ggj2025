#include "HeroHudEntity.h"

#include <gf/Anchor.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/StringUtils.h>
#include <gf/Text.h>

#include "GameHub.h"
#include "GameState.h"

namespace be {
  HeroHudEntity::HeroHudEntity(GameHub& game)
  : m_state(game.state)
  , m_font(game.resources.getFont("DejaVuSans.ttf"))
  {
  }

  void HeroHudEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Coordinates coords(target);

    const float bubbleValue = m_state.computeBubblesValues();
    gf::Text text("Cargo: " + gf::niceNum(bubbleValue, 1.0f) + " / " + gf::niceNum(m_state.contract.bubbleValueTarget, 1.0f), m_font);
    text.setColor(gf::Color::White);
    text.setAnchor(gf::Anchor::CenterLeft);
    text.setPosition(coords.getRelativePoint({0.1f, 0.1f}));
    target.draw(text, states);
  }

}
