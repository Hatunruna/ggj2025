#include "CityEntity.h"

#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/Direction.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

#include "GameHub.h"
#include "MapSettings.h"

namespace be {

  CityEntity::CityEntity(GameHub& game)
  : m_state(game.state)
  , m_texture(game.resources.getTexture("city.png"))
  , m_font(game.resources.getFont("DejaVuSans.ttf"))
  , m_upGateTexture(game.resources.getTexture("door_up.png"))
  , m_downGateTexture(game.resources.getTexture("door_down.png"))
  , m_leftGateTexture(game.resources.getTexture("door_left.png"))
  , m_rightGateTexture(game.resources.getTexture("door_right.png"))
  {
  }

  void CityEntity::getGateTexture(gf::Direction direction) {
    switch (direction)
    {
    case gf::Direction::Up:
      m_currentGateTexture = &m_upGateTexture;
      break;
    
    case gf::Direction::Down:
      m_currentGateTexture = &m_downGateTexture;
      break;
    
    case gf::Direction::Left:
      m_currentGateTexture = &m_leftGateTexture;
      break;
    
    case gf::Direction::Right:
      m_currentGateTexture = &m_rightGateTexture;
      break;
    
    default:
      assert(false);
      break;
    }
  }

  void CityEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Coordinates coords(target);

    gf::Sprite sprite(m_texture);
    sprite.setAnchor(gf::Anchor::Center);

    for (int i = 0; i < int(m_state.cities.size()); ++i) {
      const auto& city = m_state.cities[i];
      // sprite.setPosition(city.location);
      // target.draw(sprite, states);

      gf::Text name(city.name, m_font, coords.getRelativeCharacterSize(0.05f));
      name.setAnchor(gf::Anchor::Center);
      if (m_state.contract.targetCity == i) {
        name.setColor(gf::Color::Red);
      } else {
        name.setColor(gf::Color::White);
      }
      name.setPosition(city.spot.location);
      target.draw(name, states);

      for (const auto& gateSpot: city.gates) {
        getGateTexture(gateSpot.direction);
        gf::Sprite gate(*m_currentGateTexture);
        gate.setAnchor(gf::Anchor::Center);
        gate.setPosition(gateSpot.spot.location);
        target.draw(gate, states);
      }
    }
  }

}
