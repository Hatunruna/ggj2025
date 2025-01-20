#ifndef BE_PHYSICS_MODEL_H
#define BE_PHYSICS_MODEL_H

#include <gf/Model.h>

#include <chipmunk/chipmunk.h>

namespace be {

  class PhysicsModel : public gf::Model {
  public:
    PhysicsModel();
    PhysicsModel(const PhysicsModel&) = delete;
    PhysicsModel(PhysicsModel&&) noexcept;
    ~PhysicsModel();

    PhysicsModel& operator=(const PhysicsModel&) = delete;
    PhysicsModel& operator=(PhysicsModel&&) noexcept;

    void update(gf::Time time) override;

    cpSpace* getSpace()
    {
      return m_space;
    }

  private:
    cpSpace* m_space = nullptr;
  };

}

#endif // BE_PHYSICS_MODEL_H
