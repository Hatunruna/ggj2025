#ifndef BE_HERO_REPORT_ENTITY_H
#define BE_HERO_REPORT_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>

namespace be {
   struct GameHub;
   struct GameState;

  class HeroReportEntity : public gf::Entity {
  public:
    HeroReportEntity(GameHub& game);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    gf::Font& m_font;
    gf::Texture& m_backgroundTexture;
  };

}

#endif // BE_HERO_REPORT_ENTITY_H
