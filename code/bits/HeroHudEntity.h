#ifndef BE_HERO_HUD_ENTITY_H
#define BE_HERO_HUD_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>

namespace be {

  struct GameHub;
  struct GameState;

  class HeroHudEntity : public gf::Entity {
  public:
    HeroHudEntity(GameHub& game);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    gf::Font& m_font;

  };

}

#endif // BE_HERO_HUD_ENTITY_H
