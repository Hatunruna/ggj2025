#ifndef BE_CITY_ENTITY_H
#define BE_CITY_ENTITY_H

#include <gf/Entity.h>
#include <gf/Direction.h>
#include <gf/Font.h>
#include <gf/Texture.h>

namespace be {
  struct GameHub;
  struct GameState;

  class CityEntity : public gf::Entity {
  public:
    CityEntity(GameHub& game);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    const gf::Texture& m_texture;
    gf::Font& m_font;
  };

}

#endif // BE_CITY_ENTITY_H
