#ifndef BE_HERO_HINT_ENTITY_H
#define BE_HERO_HINT_ENTITY_H

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/Texture.h>

namespace be {
  struct GameHub;
  struct GameState;

  class HeroHintEntity : public gf::Entity {
  public:
    HeroHintEntity(GameHub& game);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    const gf::Texture& m_arrowTexture;
  };


}

#endif // BE_HERO_HINT_ENTITY_H
