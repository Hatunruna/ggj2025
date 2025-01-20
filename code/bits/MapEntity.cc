#include "MapEntity.h"

#include <gf/RenderTarget.h>
#include <gf/TileLayer.h>

#include "GameHub.h"
#include "MapSettings.h"

namespace be {

  MapEntity::MapEntity(GameHub& game)
  : m_state(game.state)
  , m_texture(game.resources.getTexture("tileset.png"))
  {
  }

  void MapEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::TileLayer tiled = gf::TileLayer::createOrthogonal(MapSize, TileSize);

    auto tilesetId = tiled.createTilesetId();
    auto& tileset = tiled.getTileset(tilesetId);

    tileset.setTexture(m_texture);
    tileset.setMargin(Padding);
    tileset.setSpacing(Margin);
    tileset.setTileSize(TileSize);

    auto& cells = m_state.map.cells;

    for (auto position : cells.getPositionRange()) {
      switch (cells(position).type) {
        case CellType::Ground:
          tiled.setTile(position, tilesetId, 0 + cells(position).tile);
          break;
        case CellType::Cliff:
          tiled.setTile(position, tilesetId, 4 + cells(position).tile);
          break;
        case CellType::Block:
          tiled.setTile(position, tilesetId, 8 + cells(position).tile);
          break;
      }
    }

    target.draw(tiled, states);
  }

}
