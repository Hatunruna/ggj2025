#include "GameState.h"
#include "MapSettings.h"

#include <gf/Log.h>
#include <gf/Geometry.h>
#include <gf/Polyline.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>
#include <gf/Streams.h>

namespace be {

  namespace {
    constexpr cpFloat HeroMass = 10.0;
    constexpr cpFloat HeroRadius = 32.0;

    constexpr cpFloat ProducerRadius = 32.0;

    constexpr cpFloat MapRadius = 0.1;

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
