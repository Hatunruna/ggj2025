#include "GameState.h"

#include <gf/Serialization.h>
#include <gf/SerializationOps.h>
#include <gf/Streams.h>

namespace be {

  namespace {
    constexpr cpFloat HeroMass = 10.0f;
    constexpr cpFloat HeroRadius = 32.0f;

    constexpr cpFloat ProducerRadius = 32.0f;

    gf::Vector2f toVec(cpVect raw) {
      return gf::vec(raw.x, raw.y);
    }

  }

  void GameState::initializePhysics()
  {
    cpSpace* space = physics.getSpace();

    /*
     * Hero
     */

    hero.body = cpBodyNew(HeroMass, cpMomentForCircle(HeroMass, 0.0f, HeroRadius, cpvzero));
    cpBodySetPosition(hero.body, cpv(hero.location.x, hero.location.y));
    cpSpaceAddBody(space, hero.body);

    cpShape *shape = cpCircleShapeNew(hero.body, HeroRadius, cpvzero);
    cpShapeSetElasticity(shape, 0.0f);
    cpShapeSetFriction(shape, 0.7f);
    cpSpaceAddShape(space, shape);

    hero.control = cpSpaceAddBody(space, cpBodyNewKinematic());

    cpConstraint *pivot = cpPivotJointNew2(hero.control, hero.body, cpvzero, cpvzero);
    cpConstraintSetMaxBias(pivot, 0); // disable joint correction
    cpConstraintSetMaxForce(pivot, 10000.0f); // emulate linear friction
    cpSpaceAddConstraint(space, pivot);

    cpConstraint *gear = cpGearJointNew(hero.control, hero.body, 0.0f, 1.0f);
    cpConstraintSetErrorBias(gear, 0); // attempt to fully correct the joint each step
    cpConstraintSetMaxBias(gear, 1.2f);  // but limit it's angular correction rate
    cpConstraintSetMaxForce(gear, 50000.0f); // emulate angular friction
    cpSpaceAddConstraint(space, gear);

    /*
     * Producers
     */

    auto producersBody = cpBodyNewStatic();
    cpSpaceAddBody(space, producersBody);

    for (auto& producer : producers) {
      auto producerShape = cpCircleShapeNew(producersBody, ProducerRadius, cpv(producer.location.x, producer.location.y));
      cpSpaceAddShape(space, producerShape);
    }

  }

  void GameState::update(gf::Time time)
  {
    physics.update(time);

    hero.location = toVec(cpBodyGetPosition(hero.body));

    for (auto& producer : producers) {
      producer.lifetime += time;

      switch (producer.status) {
        case BubbleProducerStatus::Emerging:
          producer.size = producer.minSize;
          producer.status = BubbleProducerStatus::Growing;
          producer.lifetime = gf::Time::zero();
          break;

        case BubbleProducerStatus::Growing:
          producer.size += producer.growthRate * time.asSeconds();

          if (producer.size > producer.maxSize) {
            producer.lifetime = gf::Time::zero();
            producer.status = BubbleProducerStatus::Exploding;
          }
          break;

        case BubbleProducerStatus::Exploding:
          if (producer.lifetime > ExplosionLifetime) {
            producer.lifetime = gf::Time::zero();
            producer.status = BubbleProducerStatus::Emerging;
          }
          break;
      }
    }
  }

  void GameState::loadFromFile(const gf::Path& filename) {
    gf::FileInputStream file(filename);
    gf::CompressedInputStream compressed(file);
    gf::Deserializer ar(compressed);
    ar | *this;
  }

  void GameState::saveToFile(const gf::Path& filename) {
    gf::FileOutputStream file(filename);
    gf::CompressedOutputStream compressed(file);
    gf::Serializer ar(compressed, Version);
    ar | *this;
  }


}
