#ifndef BE_BUBBLE_ENTITY_H
#define BE_BUBBLE_ENTITY_H

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/Texture.h>

namespace be {
  struct GameHub;
  struct GameState;

  class BubbleEntity : public gf::Entity {
  public:
    BubbleEntity(GameHub& game);

    void update(gf::Time time) override;
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    const gf::Texture& m_producerTexture;
    const gf::Texture& m_redBubbleTexture;
    gf::Animation m_redBubbleAnimation;
    const gf::Texture& m_blueBubbleTexture;
    gf::Animation m_blueBubbleAnimation;
    const gf::Texture& m_greenBubbleTexture;
    gf::Animation m_greenBubbleAnimation;
    const gf::Texture& m_yellowBubbleTexture;
    gf::Animation m_yellowBubbleAnimation;
  };


}

#endif // BE_BUBBLE_ENTITY_H
