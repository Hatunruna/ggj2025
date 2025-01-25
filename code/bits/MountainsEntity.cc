#include "MountainsEntity.h"

#include <gf/Rect.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/SpriteBatch.h>

#include "GameHub.h"
#include "MapSettings.h"

namespace be {

  MountainsEntity::MountainsEntity(GameHub& game)
  : m_state(game.state)
  , m_texture(game.resources.getTexture("mountains_tiles.png"))
  {
  }

  void MountainsEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::SpriteBatch batch(target);

    batch.begin();
    auto& cells = m_state.map.cells;
    for (auto position : cells.getPositionRange()) {
      switch (cells(position).type) {
        case CellType::Ground:
        case CellType::Block:
          break;
        case CellType::Cliff:
          constexpr float TileWidth = 1.0f / 7.0f;
          gf::Sprite sprite(m_texture, gf::RectF::fromPositionSize(gf::vec(TileWidth * cells(position).tile, 0.0f), {TileWidth, 1.0f}));
          sprite.setAnchor(gf::Anchor::TopLeft);
          sprite.setScale(0.5f);
          sprite.setPosition((position + gf::vec(0, 0)) * TileSize);
          batch.draw(sprite, states);
          break;
      }
    }
    batch.end();

  }

}
