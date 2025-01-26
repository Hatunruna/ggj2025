#ifndef BE_TRAPS_STATE_H
#define BE_TRAPS_STATE_H

#include <gf/Entity.h>

namespace be {

  struct GameHub;
  struct GameState;

  struct TrapEntity : public gf::Entity {
  public:
    TrapEntity(GameHub& game);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
  };

}

#endif // BE_TRAPS_STATE_H
