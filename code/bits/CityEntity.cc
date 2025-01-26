#include "CityEntity.h"

#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/Direction.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

#include "GameHub.h"

namespace be {

  CityEntity::CityEntity(GameHub& game)
  : m_state(game.state)
  , m_texture(game.resources.getTexture("city.png"))
  , m_font(game.resources.getFont("DejaVuSans.ttf"))
  {
  }

  void CityEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Coordinates coords(target);

    for (int i = 0; i < int(m_state.cities.size()); ++i) {
      const auto& city = m_state.cities[i];

      gf::Text name(city.name, m_font, coords.getRelativeCharacterSize(0.05f));
      name.setAnchor(gf::Anchor::Center);
      if (m_state.contract.targetCity == i) {
        name.setColor(gf::Color::Red);
      } else {
        name.setColor(gf::Color::White);
      }
      name.setPosition(city.spot.location);
      target.draw(name, states);
    }
  }

}
