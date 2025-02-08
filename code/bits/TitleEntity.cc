#include "TitleEntity.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

namespace be {

  TitleEntity::TitleEntity(gf::ResourceManager& resources)
  : m_font(resources.getFont("GoudyBookletter1911.otf"))
  , m_backgroundTexture(resources.getTexture("logo.jpg"))
  {
    m_backgroundTexture.setSmooth();
  }

  void TitleEntity::update([[maybe_unused]] gf::Time time) {
  }

  void TitleEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);

    float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
    float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;

    gf::Sprite background(m_backgroundTexture);
    background.setPosition(coords.getCenter());
    background.setAnchor(gf::Anchor::Center);
    background.setScale(backgroundScale);
    target.draw(background, states);

    unsigned instructionsCharacterSize = coords.getRelativeCharacterSize(0.06f);

    gf::Text instructions("Press space to start", m_font, instructionsCharacterSize);
    instructions.setColor(gf::Color::White);
    instructions.setPosition(coords.getRelativePoint({ 0.5f, 0.9f }));
    instructions.setAnchor(gf::Anchor::Center);
    target.draw(instructions, states);
  }

}
