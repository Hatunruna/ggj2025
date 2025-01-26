#ifndef BE_TRAPS_STATE_H
#define BE_TRAPS_STATE_H

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/Texture.h>

namespace be {

  struct GameHub;
  struct GameState;

  struct TrapEntity : public gf::Entity {
  public:
    TrapEntity(GameHub& game);

    void update(gf::Time time) override;
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    gf::Texture& m_spawnTileset;
    gf::Texture& m_triggerTileset;
    gf::Texture& m_growTileset;
    gf::Texture& m_activeTileset;
    gf::Texture& m_restTileset;

    gf::Animation m_spawnAnimation;
    gf::Animation m_triggerAnimation;
    gf::Animation m_growAnimation;
    gf::Animation m_activeAnimation;
    gf::Animation m_restAnimation;
  };

}

#endif // BE_TRAPS_STATE_H
