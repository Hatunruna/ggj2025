#include "MapEntity.h"

#include <gf/RenderTarget.h>

#include "GameHub.h"
#include "MapSettings.h"

namespace be {

  MapEntity::MapEntity(GameHub& game)
  : m_state(game.state)
  , m_texture(game.resources.getTexture("ground_tiles.png"))
  , m_tiled(gf::TileLayer::createOrthogonal(MapSize, TileSize * 2))
  {
  }

  void MapEntity::updateTiles()
  {
    m_tiled.clear();

    auto tilesetId = m_tiled.createTilesetId();
    auto& tileset = m_tiled.getTileset(tilesetId);

    tileset.setTexture(m_texture);
    // tileset.setMargin(Padding);
    // tileset.setSpacing(Margin);
    tileset.setTileSize(TileSize * 2);

    auto& cells = m_state.map.cells;

    for (auto position : cells.getPositionRange()) {
      switch (cells(position).type) {
        case CellType::Ground:
          m_tiled.setTile(position, tilesetId, GroundTilesetWidth * 0 + cells(position).tile);
          break;
        case CellType::Cliff:
        case CellType::Block:
          break;
      }
    }

    m_tiled.scale(0.5f);
  }

  void MapEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    target.draw(m_tiled, states);
  }

}
