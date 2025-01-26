#include "MiniMapEntity.h"

#include <gf/Sprite.h>
#include <gf/Vector.h>

#include "GameHub.h"
#include "MapSettings.h"
#include "MapState.h"
#include "Support.h"

namespace be {

  MiniMapEntity::MiniMapEntity(GameHub& game)
  : m_state(game.state)
  , m_mapTexture(MapSize)
  , m_fogTexture(MapSize)
  {
  }

  void MiniMapEntity::updateMiniMap()
  {
    gf::Image image(m_state.map.cells.getSize(), gf::Color::toRgba32(gf::Color::Black));
    assert(image.getSize() == MapSize);

    for (auto position : m_state.map.cells.getPositionRange())
    {
      switch (m_state.map.cells(position).type) {
        case CellType::Ground:
          image.setPixel(position, { 0x3C, 0x96, 0xee, 0xFF });
          break;
        case CellType::Cliff:
          image.setPixel(position, { 0xA8, 0x00, 0x25, 0xFF });
          break;
        case CellType::Block:
          image.setPixel(position, { 0x00, 0x00, 0x00, 0xFF });
          break;
      }
    }

    m_mapTexture.update(image);
  }

  void MiniMapEntity::updateFogOfWar()
  {
    gf::Image image(m_state.map.cells.getSize(), gf::Color::toRgba32(gf::Color::Black));
    assert(image.getSize() == MapSize);

    for (auto position : m_state.miniMap.visited.getPositionRange()) {
      if (m_state.miniMap.visited(position) == MiniMapStatus::Visited) {
        image.setPixel(position, gf::Color::toRgba32(gf::Color::Transparent));
      }
    }

    const auto heroPosition = gf::Vector2i(m_state.hero.location / TileSize);
    const auto points = generateCircle(heroPosition, 5);
    for (const auto& point: points) {
      image.setPixel(point, { 0xFF, 0xFF, 0x00, 0xFF});
    }

    m_fogTexture.update(image);
  }

  void MiniMapEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Sprite mapSprite(m_mapTexture);
    mapSprite.setAnchor(gf::Anchor::Center);
    mapSprite.setPosition({ 0.0f, 0.0f });
    target.draw(mapSprite, states);

    gf::Sprite fogSprite(m_fogTexture);
    fogSprite.setAnchor(gf::Anchor::Center);
    fogSprite.setPosition({ 0.0f, 0.0f });
    target.draw(fogSprite, states);
  }


}
