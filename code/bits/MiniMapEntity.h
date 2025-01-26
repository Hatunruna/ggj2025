#ifndef BE_MINI_MAP_ENTITY_H
#define BE_MINI_MAP_ENTITY_H

#include <gf/Entity.h>
#include <gf/Texture.h>

namespace be {
  struct GameHub;
  struct GameState;

  class MiniMapEntity : public gf::Entity {
  public:
    MiniMapEntity(GameHub& game);

    void updateMiniMap();
    void updateFogOfWar();

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    gf::Texture m_mapTexture;
    gf::Texture m_fogTexture;
  };

}

#endif // BE_MINI_MAP_ENTITY_H
