#ifndef BE_HERO_ENTITY_H
#define BE_HERO_ENTITY_H

#include <gf/Entity.h>
#include <gf/Texture.h>

namespace be {
  struct GameHub;
  struct GameState;

  class HeroEntity : public gf::Entity {
  public:
    HeroEntity(GameHub& game);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    const gf::Texture& m_texture;

  };


}

#endif // BE_HERO_ENTITY_H
