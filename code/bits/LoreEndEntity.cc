#include "LoreEndEntity.h"

#include <string_view>

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

#include "GameHub.h"
#include "gf/Alignment.h"

namespace be {

  LoreEndEntity::LoreEndEntity(GameHub& game)
  : m_game(game)
  , m_font(game.resources.getFont("DejaVuSans.ttf"))
  {
  }

  void LoreEndEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Coordinates coords(target);

    constexpr float characterSize = 0.04f;
    const unsigned relativeCharacterSize = coords.getRelativeCharacterSize(characterSize);

    gf::Text text("Press space to start the game", m_font, relativeCharacterSize);
    text.setAnchor(gf::Anchor::Center);
    text.setColor(gf::Color::Gray());
    text.setPosition(coords.getRelativePoint({ 0.5f, 0.9f }));
    target.draw(text, states);
  }

}
