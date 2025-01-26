#include "TrapEntity.h"

#include <gf/Anchor.h>
#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

#include "GameHub.h"

namespace be {

  TrapEntity::TrapEntity(GameHub& game)
  : m_state(game.state)
  {

  }

  void TrapEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    for (const auto& trap: m_state.traps) {
      gf::RectangleShape rect({32.0f, 32.0f});
      rect.setColor(gf::Color::Magenta);
      rect.setAnchor(gf::Anchor::Center);
      rect.setPosition(trap.spot.location);
      target.draw(rect, states);
    }
  }

}
