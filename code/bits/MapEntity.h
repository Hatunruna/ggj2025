#ifndef BE_MAP_ENTITY_H
#define BE_MAP_ENTITY_H

#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/TileLayer.h>

namespace be {
  struct GameHub;
  struct GameState;

  class MapEntity : public gf::Entity {
  public:
    MapEntity(GameHub& game);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    void updateTiles();

  private:
    GameState& m_state;
    gf::Texture& m_texture;
    gf::TileLayer m_tiled;
  };

}

#endif // BE_MAP_ENTITY_H
