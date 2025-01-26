#include "GateEntity.h"

#include <gf/Sprite.h>

#include "GameHub.h"
#include "MapSettings.h"

namespace be {

  GateEntity::GateEntity(GameHub& game)
  : m_state(game.state)
  , m_downGateTexture(game.resources.getTexture("door_down.png"))
  , m_sideGateTexture(game.resources.getTexture("door_side.png"))
  {
    m_downGateTexture.setSmooth();
    m_sideGateTexture.setSmooth();
  }

  void GateEntity::render(gf::RenderTarget& target, const gf::RenderStates& states)
  {
    for (const auto& city : m_state.cities) {
      for (const auto& gate: city.gates) {
        gf::Sprite sprite;

        gf::Vector2i offset(0, 0);
        switch (gate.direction) {
          case gf::Direction::Left:
            sprite.setTexture(m_sideGateTexture);
            sprite.setScale(gf::vec(0.5f, 0.5f));
            offset.x =  1;
            offset.y = -2;
            break;

          case gf::Direction::Right:
            sprite.setTexture(m_sideGateTexture);
            sprite.setScale(gf::vec(-0.5f, 0.5f));
            offset.x =  0;
            offset.y = -2;
            break;

          case gf::Direction::Down:
            sprite.setTexture(m_downGateTexture);
            sprite.setScale(0.5f);
            offset.x = -1;
            offset.y = -4;
            break;

          default:
            assert(false);
        }
        sprite.setPosition((gate.spot.position - gf::displacement(gate.direction) + offset) * TileSize);
        target.draw(sprite, states);

        // switch (gate.direction) {
        //   case gf::Direction::Left:
        //   case gf::Direction::Right:
        //   {
        //     gf::RectangleShape debug(TileSize * gf::vec(1, 5));
        //     debug.setAnchor(gf::Anchor::TopLeft);
        //     debug.setPosition((gate.spot.position + gf::vec(0, -2)) * TileSize);
        //     debug.setColor(gf::Color::Cyan);
        //     target.draw(debug, states);

        //     debug.setSize(TileSize);
        //     debug.setAnchor(gf::Anchor::TopLeft);
        //     debug.setPosition(gate.spot.position * TileSize);
        //     debug.setColor(gf::Color::Yellow);
        //     target.draw(debug, states);
        //   }
        //     break;

        //   case gf::Direction::Down:
        //   {
        //     gf::RectangleShape debug(TileSize * gf::vec(4, 1));
        //     debug.setAnchor(gf::Anchor::TopLeft);
        //     debug.setPosition((gate.spot.position + gf::vec(-1, 0)) * TileSize);
        //     debug.setColor(gf::Color::Cyan);
        //     target.draw(debug, states);

        //     debug.setSize(TileSize);
        //     debug.setAnchor(gf::Anchor::TopLeft);
        //     debug.setPosition(gate.spot.position * TileSize);
        //     debug.setColor(gf::Color::Yellow);
        //     target.draw(debug, states);
        //   }
        //     break;

        //   default:
        //     assert(false);
        // }
      }
    }
  }

}
