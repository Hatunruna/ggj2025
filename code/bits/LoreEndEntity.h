#ifndef BE_LORE_ENTITY_END_H
#define BE_LORE_ENTITY_END_H

#include <gf/Entity.h>
#include <gf/Font.h>

namespace be {
  struct GameHub;

  class LoreEndEntity : public gf::Entity {
  public:
    LoreEndEntity(GameHub& game);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameHub& m_game;
    gf::Font& m_font;
  };

}

#endif // BE_LORE_ENTITY_END_H
