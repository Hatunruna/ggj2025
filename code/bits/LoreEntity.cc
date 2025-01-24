#include "LoreEntity.h"

#include <string_view>

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

#include "GameHub.h"
#include "gf/Alignment.h"

namespace be {

  namespace {

    constexpr std::string_view Lore =
    "2301, Nova Franca.\n"
    "Many years after the great collapse, humans now live in underworld cities that use a new energy to survive: thalladium.\n"
    "Thalladium must be captured in the bubbles that appear at the surface of the planet. Freelancers are hired by Bubble Express to harvest the bubbles and deliver them to the cities.\n"
    "You are one of these freelancers, and you must find and harvest bubbles to fulfill the contracts of Bubble Express.\n"
    "The upper world is a dangerous place, beware."
    ;

  }

  LoreEntity::LoreEntity(GameHub& game)
  : m_game(game)
  , m_font(game.resources.getFont("DejaVuSans.ttf"))
  {
  }

  void LoreEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Coordinates coords(target);

    constexpr float characterSize = 0.04f;
    const unsigned relativeCharacterSize = coords.getRelativeCharacterSize(characterSize);

    const float paragraphWidth = coords.getRelativeSize({ 0.5f, 0.0f }).width;

    gf::Text text(std::string(Lore), m_font, relativeCharacterSize);
    text.setParagraphWidth(paragraphWidth);
    text.setAlignment(gf::Alignment::Justify);
    text.setAnchor(gf::Anchor::Center);
    text.setColor(gf::Color::White);
    text.setPosition(coords.getRelativePoint({ 0.5f, 0.45f }));
    target.draw(text, states);
  }

}
