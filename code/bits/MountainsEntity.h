#ifndef BE_MOUNTAINS_ENTITY_H
#define BE_MOUNTAINS_ENTITY_H

#include <gf/Entity.h>
#include <gf/Texture.h>
#include <gf/TileLayer.h>

namespace be {
  struct GameHub;
  struct GameState;

  class MountainsEntity : public gf::Entity {
  public:
    MountainsEntity(GameHub& game);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    gf::Texture& m_texture;
  };

}

#endif // BE_MOUNTAINS_ENTITY_H
