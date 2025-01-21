#include "CityEntity.h"

#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

#include "GameHub.h"

namespace be {

  CityEntity::CityEntity(GameHub& game)
  : m_state(game.state)
  , m_texture(game.resources.getTexture("city.png"))
  {
  }

  void CityEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Sprite sprite(m_texture);
    sprite.setAnchor(gf::Anchor::Center);

    for (auto& city : m_state.cities) {
      sprite.setPosition(city.location);
      target.draw(sprite, states);
    }
  }

}
