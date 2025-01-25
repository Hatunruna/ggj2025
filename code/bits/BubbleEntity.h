#ifndef BE_BUBBLE_ENTITY_H
#define BE_BUBBLE_ENTITY_H

#include <gf/Entity.h>
#include <gf/Texture.h>

namespace be {
  struct GameHub;
  struct GameState;

  class BubbleEntity : public gf::Entity {
  public:
    BubbleEntity(GameHub& game);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    const gf::Texture& m_producerTexture;
    const gf::Texture& m_redBubbleTexture;
    const gf::Texture& m_blueBubbleTexture;
    const gf::Texture& m_greenBubbleTexture;
    const gf::Texture& m_yellowBubbleTexture;
  };


}

#endif // BE_BUBBLE_ENTITY_H
