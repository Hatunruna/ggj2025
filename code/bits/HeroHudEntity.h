#ifndef BE_HERO_HUD_ENTITY_H
#define BE_HERO_HUD_ENTITY_H

#include <gf/Animation.h>
#include <gf/Entity.h>
#include <gf/Font.h>

namespace be {

  struct GameHub;
  struct GameState;

  class HeroHudEntity : public gf::Entity {
  public:
    HeroHudEntity(GameHub& game);
    float HeroHudBubbleScale = 0.3f;

    void update(gf::Time time) override;
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    gf::Font& m_font;
    gf::Animation* m_contractBubble;

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

#endif // BE_HERO_HUD_ENTITY_H
