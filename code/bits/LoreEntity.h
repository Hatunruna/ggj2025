#ifndef BE_LORE_ENTITY_H
#define BE_LORE_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>

namespace be {
  struct GameHub;

  class LoreEntity : public gf::Entity {
  public:
    LoreEntity(GameHub& game);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameHub& m_game;
    gf::Font& m_font;
  };

}

#endif // BE_LORE_ENTITY_H
