#ifndef BE_FEEDBACK_ENTITY_H
#define BE_FEEDBACK_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>

namespace be {

  struct GameHub;
  struct GameState;

  class FeedbackEntity : public gf::Entity {
  public:
    FeedbackEntity(GameHub& game);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    gf::Font& m_font;

  };

}

#endif // BE_FEEDBACK_ENTITY_H
