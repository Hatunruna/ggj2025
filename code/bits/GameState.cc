#include "GameState.h"
#include "MapSettings.h"

#include <gf/Log.h>
#include <gf/Geometry.h>
#include <gf/Polyline.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>
#include <gf/Streams.h>
#include <gf/VectorOps.h>

namespace be {

  namespace {
    constexpr cpFloat HeroMass = 10.0;
    constexpr cpFloat HeroRadius = 32.0;

    constexpr cpFloat ProducerRadius = 32.0;
    constexpr cpFloat GateRadius = 32.0;

    constexpr cpFloat MapRadius = 0.1;

    constexpr cpFloat BubbleMass = 1.0;
    constexpr cpFloat BubbleRadius = 64.0;
    constexpr cpFloat BubbleRelaxation = 1.5;

    constexpr float ProducerDistance = HeroRadius + ProducerRadius + 50.0f;
    constexpr float GateDistance = HeroRadius + GateRadius + 50.0f;

    gf::Vector2f toVec(cpVect raw) {
      return gf::vec(raw.x, raw.y);
    }

    std::vector<gf::Polyline> computeMapCollisions(const MapState& map)
    {
      std::vector<gf::SegmentI> segments;

      for (auto position : map.cells.getPositionRange()) {
        if (map.cells(position).type != CellType::Ground) {
          continue;
        }

        const gf::Vector2i base = position * TileSize;

        const gf::Vector2i up = position + gf::diry(-1);

        if (map.cells.isValid(up) && map.cells(up).type != CellType::Ground) {
          gf::SegmentI segment = {};
          segment.p0 = base;
          segment.p1 = base + gf::dirx(TileSize.x);
          segments.push_back(segment);
        }

        const gf::Vector2i left = position + gf::dirx(-1);

        if (map.cells.isValid(left) && map.cells(left).type != CellType::Ground) {
          gf::SegmentI segment = {};
          segment.p0 = base;
          segment.p1 = base + gf::diry(TileSize.y);
          segments.push_back(segment);
        }

        const gf::Vector2i down = position + gf::diry(+1);

        if (map.cells.isValid(down) && map.cells(down).type != CellType::Ground) {
          gf::SegmentI segment = {};
          segment.p0 = base + gf::diry(TileSize.y);
          segment.p1 = base + gf::diry(TileSize.y) + gf::dirx(TileSize.x);
          segments.push_back(segment);
        }

        const gf::Vector2i right = position + gf::dirx(+1);

        if (map.cells.isValid(right) && map.cells(right).type != CellType::Ground) {
          gf::SegmentI segment = {};
          segment.p0 = base + gf::dirx(TileSize.x);
          segment.p1 = base + gf::dirx(TileSize.x) + gf::diry(TileSize.y);
          segments.push_back(segment);
        }
      }

      gf::Log::debug("Segments: %zu\n", segments.size());

      return gf::buildLines(segments);
    }

  }

  void GameState::initializePhysics()
  {
    cpSpace* space = physics.getSpace();

    /*
     * Map
     */

    auto mapBody = cpBodyNewStatic();
    cpSpaceAddBody(space, mapBody);
    auto mapCollisions = computeMapCollisions(map);

    gf::Log::debug("Collisions: %zu\n", mapCollisions.size());

    for (auto& polyline : mapCollisions) {
      const std::size_t last = polyline.getPointCount() - 1;

      for (std::size_t i = 0; i < last; ++i) {
        const gf::Vector2i curr = polyline.getPoint(i);
        const gf::Vector2i next = polyline.getNextPoint(i);
        const gf::Vector2i currNeighbor = polyline.hasPrevPoint(i) ? static_cast<const gf::Vector2i>(polyline.getPrevPoint(i)) : curr;
        const gf::Vector2i nextNeighbor = polyline.hasNextPoint(i + 1) ? static_cast<const gf::Vector2i>(polyline.getNextPoint(i + 1)) : next;

        auto shape = cpSegmentShapeNew(mapBody, cpv(curr.x, curr.y), cpv(next.x, next.y), MapRadius);
        cpSegmentShapeSetNeighbors(shape, cpv(currNeighbor.x, currNeighbor.y), cpv(nextNeighbor.x, nextNeighbor.y));
        cpSpaceAddShape(space, shape);
      }

      if (polyline.isLoop()) {
        const gf::Vector2i curr = polyline.getPoint(last);
        const gf::Vector2i next = polyline.getPoint(0);
        const gf::Vector2i currNeighbor = polyline.getPrevPoint(last);
        const gf::Vector2i nextNeighbor = polyline.getNextPoint(0);

        auto shape = cpSegmentShapeNew(mapBody, cpv(curr.x, curr.y), cpv(next.x, next.y), MapRadius);
        cpSegmentShapeSetNeighbors(shape, cpv(currNeighbor.x, currNeighbor.y), cpv(nextNeighbor.x, nextNeighbor.y));
        cpSpaceAddShape(space, shape);
      }
    }

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

    /*
     * Bubbles
     */

    for (auto& bubble : bubbles) {
      bubble.body = cpBodyNew(BubbleMass, cpMomentForCircle(BubbleMass, 0.0, bubble.size * BubbleRadius, cpvzero));
      cpBodySetVelocity(bubble.body, cpvzero);
      cpSpaceAddBody(space, bubble.body);

      cpShape* shape = cpCircleShapeNew(bubble.body, bubble.size * BubbleRadius, cpvzero);
      cpShapeSetElasticity(shape, 0.0f);
      cpShapeSetFriction(shape, 0.7f);
      cpSpaceAddShape(space, shape);

      bubble.pin = cpSlideJointNew(hero.body, bubble.body, cpvzero, cpvzero, (HeroRadius + bubble.size * BubbleRadius), (HeroRadius + bubble.size * BubbleRadius) * BubbleRelaxation);
      cpSpaceAddConstraint(space, bubble.pin);
    }


  }

  void GameState::moveHero(gf::Vector2i direction)
  {
    if (direction != gf::vec(0, 0)) {
      // gf::Log::debug("direction: %i,%i\n", direction.x, direction.y);

      const gf::Vector2f unit = gf::normalize(gf::Vector2f(direction));
      const float rotation = gf::angle(unit);
      const gf::Vector2f velocity = HeroVelocity * unit;

      cpBodySetAngle(hero.body, rotation);
      cpBodySetAngle(hero.control, rotation);
      cpBodySetVelocity(hero.control, cpv(velocity.x, velocity.y));
    } else {
      cpBodySetVelocity(hero.control, cpvzero);
    }
  }

  void GameState::teleportHero(gf::Vector2f position)
  {
    cpBodySetPosition(hero.body, cpv(position.x, position.y));
    cpBodySetPosition(hero.control, cpv(position.x, position.y));
    cpBodySetVelocity(hero.control, cpvzero);
  }

  void GameState::tryToTakeBubble()
  {
    cpSpace* space = physics.getSpace();

    for (auto& producer : producers) {
      if (gf::squareDistance(producer.location, hero.location) > gf::square(ProducerDistance)) {
        continue;
      }

      gf::Log::debug("Bubble taken!\n");

      BubbleState bubble = {};

      bubble.size = producer.size;

      const gf::Vector2f location = producer.location - gf::diry(bubble.size * BubbleRadius);

      bubble.body = cpBodyNew(BubbleMass, cpMomentForCircle(BubbleMass, 0.0, bubble.size * BubbleRadius, cpvzero));
      cpBodySetVelocity(bubble.body, cpvzero);
      cpBodySetPosition(bubble.body, cpv(location.x, location.y));
      cpSpaceAddBody(space, bubble.body);

      cpShape* shape = cpCircleShapeNew(bubble.body, bubble.size * BubbleRadius, cpvzero);
      cpShapeSetElasticity(shape, 0.0f);
      cpShapeSetFriction(shape, 0.7f);
      cpSpaceAddShape(space, shape);

      bubble.pin = cpSlideJointNew(hero.body, bubble.body, cpvzero, cpvzero, (HeroRadius + bubble.size * BubbleRadius), (HeroRadius + bubble.size * BubbleRadius) * BubbleRelaxation);
      cpSpaceAddConstraint(space, bubble.pin);

      bubbles.push_back(bubble);

      producer.status = BubbleProducerStatus::Emerging;
      return;
    }
  }

  bool GameState::tryToEnterCity()
  {
    cpSpace* space = physics.getSpace();

    const auto& city = cities[contract.targetCity];
    for (const auto& gate: city.gates) {
      if (gf::squareDistance(gate, hero.location) > gf::square(ProducerDistance)) {
        continue;
      }

      contract.originCity = contract.targetCity;
      contract.targetCity = -1;

      gf::Log::debug("Entering the city\n");
      return true;
    }

    return false;
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
