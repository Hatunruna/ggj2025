#ifndef BE_HERO_ENTITY_H
#define BE_HERO_ENTITY_H

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/Texture.h>

namespace be {
  struct GameHub;
  struct GameState;

  constexpr float ArrowSize = 10.0f;
  constexpr float ArrowRange = 128.0f;
  constexpr float HeroScale = 0.5f;

  class HeroEntity : public gf::Entity {
  public:
    HeroEntity(GameHub& game);
    
    void update(gf::Time time) override;
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    gf::Animation* m_currentAnimation = nullptr;
    const gf::Texture& m_runUpTexture;
    gf::Animation m_runUpAnimation;
    const gf::Texture& m_runDownTexture;
    gf::Animation m_runDownAnimation;
    const gf::Texture& m_runLeftTexture;
    gf::Animation m_runLeftAnimation;
    const gf::Texture& m_runUpLeftTexture;
    gf::Animation m_runUpLeftAnimation;
    const gf::Texture& m_runDownRightTexture;
    gf::Animation m_runDownRightAnimation;
    const gf::Texture& m_idleTexture;
    gf::Animation m_idleAnimation;
  };


}

#endif // BE_HERO_ENTITY_H
