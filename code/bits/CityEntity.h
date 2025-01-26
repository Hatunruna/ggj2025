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
    void getGateTexture(gf::Direction direction);
  
    GameState& m_state;
    const gf::Texture& m_texture;
    gf::Font& m_font;

    gf::Texture& m_upGateTexture;
    gf::Texture& m_downGateTexture;
    gf::Texture& m_leftGateTexture;
    gf::Texture& m_rightGateTexture;
    gf::Texture* m_currentGateTexture = nullptr;
  };

}

#endif // BE_CITY_ENTITY_H
