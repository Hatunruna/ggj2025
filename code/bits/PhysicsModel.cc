#include "PhysicsModel.h"

#include <utility>

namespace be {

  namespace {

    void shapeFreeWrap(cpSpace* space, void* raw, [[maybe_unused]] void* data)
    {
      auto shape = static_cast<cpShape*>(raw);
      cpSpaceRemoveShape(space, shape);
      cpShapeFree(shape);
    }

    static void postShapeFree(cpShape* shape, void* data)
    {
      auto space = static_cast<cpSpace*>(data);
      cpSpaceAddPostStepCallback(space, shapeFreeWrap, shape, nullptr);
    }

    static void constraintFreeWrap(cpSpace* space, void* raw, [[maybe_unused]] void* data)
    {
      auto constraint = static_cast<cpConstraint*>(raw);
      cpSpaceRemoveConstraint(space, constraint);
      cpConstraintFree(constraint);
    }

    static void postConstraintFree(cpConstraint* constraint, void* data)
    {
      auto space = static_cast<cpSpace*>(data);
      cpSpaceAddPostStepCallback(space, constraintFreeWrap, constraint, nullptr);
    }

    static void bodyFreeWrap(cpSpace* space, void* raw, [[maybe_unused]] void* data)
    {
      auto body = static_cast<cpBody*>(raw);
      cpSpaceRemoveBody(space, body);
      cpBodyFree(body);
    }

    static void postBodyFree(cpBody* body, void* data)
    {
      auto space = static_cast<cpSpace*>(data);
      cpSpaceAddPostStepCallback(space, bodyFreeWrap, body, nullptr);
    }


  }

  PhysicsModel::PhysicsModel()
  : m_space(cpSpaceNew())
  {
  }

  PhysicsModel::PhysicsModel(PhysicsModel&& other) noexcept
  : m_space(std::exchange(other.m_space, nullptr))
  {
  }

  PhysicsModel::~PhysicsModel()
  {
    cpSpaceEachShape(m_space, postShapeFree, m_space);
    cpSpaceEachConstraint(m_space, postConstraintFree, m_space);
    cpSpaceEachBody(m_space, postBodyFree, m_space);

    cpSpaceFree(m_space);
  }

  PhysicsModel& PhysicsModel::operator=(PhysicsModel&& other) noexcept
  {
    std::swap(other.m_space, m_space);
    return *this;
  }

  void PhysicsModel::update(gf::Time time)
  {
    cpSpaceStep(m_space, time.asSeconds());
  }

}
