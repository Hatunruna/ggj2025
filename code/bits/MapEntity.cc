#include "MapEntity.h"

#include <gf/RenderTarget.h>

#include "GameHub.h"
#include "MapSettings.h"

namespace be {

  MapEntity::MapEntity(GameHub& game)
  : m_state(game.state)
  , m_texture(game.resources.getTexture("tileset.png"))
  , m_tiled(gf::TileLayer::createOrthogonal(MapSize, TileSize))
  {
  }

  void MapEntity::updateTiles()
  {
    m_tiled.clear();

    auto tilesetId = m_tiled.createTilesetId();
    auto& tileset = m_tiled.getTileset(tilesetId);

    tileset.setTexture(m_texture);
    tileset.setMargin(Padding);
    tileset.setSpacing(Margin);
    tileset.setTileSize(TileSize);

    auto& cells = m_state.map.cells;

    for (auto position : cells.getPositionRange()) {
      switch (cells(position).type) {
        case CellType::Ground:
          m_tiled.setTile(position, tilesetId, 0 + cells(position).tile);
          break;
        case CellType::Cliff:
          m_tiled.setTile(position, tilesetId, 4 + cells(position).tile);
          break;
        case CellType::Block:
          m_tiled.setTile(position, tilesetId, 8 + cells(position).tile);
          break;
      }
    }
  }

  void MapEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    target.draw(m_tiled, states);
  }

}
