#ifndef BE_PHYSICS_DEBUG_H
#define BE_PHYSICS_DEBUG_H

#include <gf/Entity.h>
#include <gf/Particles.h>

namespace be {
  struct GameHub;

  class PhysicsDebug : public gf::Entity {
  public:
    PhysicsDebug(GameHub& game);

    void toggleDebug()
    {
      m_debug = !m_debug;
    }

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameHub& m_game;
    bool m_debug = false;
    gf::ShapeParticles m_particles;
  };

}

#endif // BE_PHYSICS_DEBUG_H
