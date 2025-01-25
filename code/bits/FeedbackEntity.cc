#include "FeedbackEntity.h"

#include <gf/Anchor.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/StringUtils.h>
#include <gf/Text.h>

#include "FeedbackState.h"
#include "GameHub.h"
#include "GameState.h"

namespace be {
  FeedbackEntity::FeedbackEntity(GameHub& game)
  : m_state(game.state)
  , m_font(game.resources.getFont("DejaVuSans.ttf"))
  {
  }

  void FeedbackEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Coordinates coords(target);

    const float characterSize = coords.getRelativeCharacterSize(0.05f);

    const auto elapsedTime = m_state.feedback.time.getElapsedTime();
    if (elapsedTime.asSeconds() < FeedbackTime) {
      gf::Text text(m_state.feedback.message, m_font, characterSize);
      text.setColor(gf::Color::White);
      text.setOutlineColor(gf::Color::Black);
      text.setOutlineThickness(characterSize * 0.01f);
      text.setAnchor(gf::Anchor::Center);
      text.setPosition(coords.getRelativePoint({0.5f, 0.5f}));
      target.draw(text, states);
    } else if (elapsedTime.asSeconds() < (FeedbackTime + FeedbackFadeTime)) {
      const float fade = ((FeedbackTime + FeedbackFadeTime) - elapsedTime.asSeconds()) / FeedbackFadeTime;
      gf::Text text(m_state.feedback.message, m_font, characterSize);
      text.setColor(gf::Color::White * gf::Color::Opaque(fade));
      text.setOutlineColor(gf::Color::Black * gf::Color::Opaque(fade));
      text.setOutlineThickness(characterSize * 0.01f);
      text.setAnchor(gf::Anchor::Center);
      text.setPosition(coords.getRelativePoint({0.5f, 0.5f}));
      target.draw(text, states);
    }
  }

}
