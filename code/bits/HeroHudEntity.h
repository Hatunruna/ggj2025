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
    float HeroHudBubbleScale = 0.3f;

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    gf::Font& m_font;
    gf::Texture& m_redBubble;
    gf::Texture& m_blueBubble;
    gf::Texture& m_greenBubble;
    gf::Texture& m_yellowBubble;
    gf::Texture* m_contractBubble;

    void setContractBubbleTexture();
  };

}

#endif // BE_HERO_HUD_ENTITY_H
