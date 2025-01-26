#ifndef BE_GATE_ENTITY_H
#define BE_GATE_ENTITY_H

#include <gf/Entity.h>
#include <gf/Texture.h>

namespace be {

  struct GameHub;
  struct GameState;

  class GateEntity: public gf::Entity {
  public:
    GateEntity(GameHub& game);

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    GameState& m_state;

    gf::Texture& m_downGateTexture;
    gf::Texture& m_sideGateTexture;
  };

}

#endif // BE_GATE_ENTITY_H
