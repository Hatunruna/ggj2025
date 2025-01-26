#include "GameState.h"

#include <cmath>

#include <chipmunk/chipmunk.h>

#include <cstddef>
#include <gf/Log.h>
#include <gf/Geometry.h>
#include <gf/Polyline.h>
#include <gf/Serialization.h>
#include <gf/SerializationOps.h>
#include <gf/Streams.h>
#include <gf/Time.h>
#include <gf/VectorOps.h>

#include "BubbleState.h"
#include "ContractState.h"
#include "HeroState.h"
#include "MapSettings.h"
#include "gf/Range.h"
#include "gf/Vector.h"

namespace be {

  namespace {
    constexpr cpFloat HeroMass = 10.0;
    constexpr cpFloat HeroRadius = 32.0;

    constexpr cpFloat ProducerRadius = 32.0;
    constexpr cpFloat GateRadius = 32.0;

    constexpr cpFloat MapRadius = 0.1;

    constexpr cpFloat BubbleMass = 1.0;
    constexpr cpFloat BubbleRadius = 100.0;
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

    gf::NeighborSquareRange<int> computeFogRange(gf::Vector2i position, gf::Vector2i size)
    {
      constexpr int N = 5;

      auto colMin = position.col - std::min(position.col, N);
      auto colMax = position.col + std::min(size.col - position.col - 1, N);
      auto rowMin = position.row - std::min(position.row, N);
      auto rowMax = position.row + std::min(size.row - position.row - 1, N);

      return gf::NeighborSquareRange<int>{ gf::Range<int>{ colMin, colMax + 1 }, gf::Range<int>{ rowMin, rowMax + 1 }, position };
    }


  }

  void attacheBubbleTo(BubbleState& bubble, cpSpace* space, cpBody* body)
  {
    bubble.pin = cpSlideJointNew(body, bubble.body, cpvzero, cpvzero, (HeroRadius + bubble.size * BubbleRadius), (HeroRadius + bubble.size * BubbleRadius) * BubbleRelaxation);
    cpSpaceAddConstraint(space, bubble.pin);
  }

  void initializeBubblePhysics(BubbleState& bubble, cpSpace* space, cpVect location, cpBody* body)
  {
    bubble.body = cpBodyNew(BubbleMass, cpMomentForCircle(BubbleMass, 0.0, bubble.size * BubbleRadius, cpvzero));
    cpBodySetVelocity(bubble.body, cpvzero);
    cpBodySetPosition(bubble.body, location);
    cpSpaceAddBody(space, bubble.body);

    bubble.shape = cpCircleShapeNew(bubble.body, bubble.size * BubbleRadius, cpvzero);
    cpShapeSetElasticity(bubble.shape, 0.0f);
    cpShapeSetFriction(bubble.shape, 0.7f);
    cpSpaceAddShape(space, bubble.shape);

    attacheBubbleTo(bubble, space, body);
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
      auto producerShape = cpCircleShapeNew(producersBody, ProducerRadius, cpv(producer.spot.location.x, producer.spot.location.y));
      cpSpaceAddShape(space, producerShape);
    }

    /*
     * Bubbles
     */

    for (auto& bubble : bubbles) {
      initializeBubblePhysics(bubble, space, cpvzero, hero.body);
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
    if (bubbles.size() >= static_cast<size_t>(HeroMaxBubble)) {
      return;
    }

    cpSpace* space = physics.getSpace();
    for (auto& producer : producers) {

      if (gf::squareDistance(producer.spot.location, hero.location) > gf::square(ProducerDistance)) {
        continue;
      }

      if (producer.type != contract.type) {
        feedback.message = "Wrong bubble type!";
        feedback.time.restart();
        return;
      }

      gf::Log::debug("Bubble taken!\n");

      BubbleState bubble = {};

      bubble.size = producer.size;
      bubble.type = producer.type;

      const gf::Vector2f location = producer.spot.location - gf::diry(bubble.size * BubbleRadius);
      initializeBubblePhysics(bubble, space, cpv(location.x, location.y), hero.body);

      bubbles.push_back(bubble);

      producer.status = BubbleProducerStatus::Emerging;
      producer.lifetime = gf::Time::zero();
      return;
    }

    auto iterator = std::find_if(freeBubbles.begin(), freeBubbles.end(), [&](const BubbleState& bubble) {
      gf::Vector2f location = toVec(cpBodyGetPosition(bubble.body));
      return gf::squareDistance(location, hero.location) <= gf::square(1.25f * (bubble.size * BubbleRadius + HeroRadius));
    });

    if (iterator != freeBubbles.end()) {
      gf::Log::debug("Old bubble taken!\n");

      BubbleState bubble = *iterator;
      freeBubbles.erase(iterator);

      attacheBubbleTo(bubble, space, hero.body);

      bubbles.push_back(bubble);
      return;
    }
  }

  void GameState::releaseBubble()
  {
    if (bubbles.empty()) {
      return;
    }

    cpSpace* space = physics.getSpace();

    BubbleState bubble = bubbles.back();
    bubbles.pop_back();

    cpSpaceRemoveConstraint(space, bubble.pin);
    cpConstraintFree(bubble.pin);
    bubble.pin = nullptr;

    freeBubbles.push_back(bubble);
  }

  void GameState::deleteAllBubbles()
  {
    cpSpace* space = physics.getSpace();

    auto deleteBubble = [&](BubbleState& bubble) {
      if (bubble.pin != nullptr) {
        cpSpaceRemoveConstraint(space, bubble.pin);
        cpConstraintFree(bubble.pin);
      }

      cpSpaceRemoveShape(space, bubble.shape);
      cpShapeFree(bubble.shape);

      cpSpaceRemoveBody(space, bubble.body);
      cpBodyFree(bubble.body);
    };

    for (auto& bubble : bubbles) {
      deleteBubble(bubble);
    }

    bubbles.clear();

    for (auto& bubble : freeBubbles) {
      deleteBubble(bubble);
    }

    freeBubbles.clear();
  }

  bool GameState::tryToEnterCity()
  {
    const auto& city = cities[contract.targetCity];
    for (const auto& gate: city.gates) {
      if (gf::squareDistance(gate.location, hero.location) > gf::square(ProducerDistance)) {
        continue;
      }

      const float bubbleValue = computeBubblesValues();
      if (bubbleValue >= contract.bubbleValueTarget || true) {
        gf::Log::debug("Entering the city\n");
        return true;
      } else {
        feedback.message = "You need more bubbles!";
        feedback.time.restart();
      }
    }

    return false;
  }

  float GameState::computeBubblesValues()
  {
    float value = 0.0f;
    for (const auto& bubble: bubbles) {
      value += bubble.size * BubbleBaseValue;
    }

    return std::round(value);
  }

  void GameState::update(gf::Time time)
  {
    physics.update(time);

    hero.location = toVec(cpBodyGetPosition(hero.body));
    hero.lastHitTime += time;

    for (auto& producer : producers) {
      producer.lifetime += time;

      switch (producer.status) {
        case BubbleProducerStatus::Emerging:
          if (producer.lifetime >= ProducerCooldown) {
            producer.size = producer.minSize;
            producer.status = BubbleProducerStatus::Growing;
            producer.lifetime = gf::Time::zero();
          }
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
            producer.size = producer.minSize;
            producer.status = BubbleProducerStatus::Growing;
          }
          break;
      }
    }

    // mini map

    gf::Vector2i heroPosition = hero.location / TileSize;
    miniMap.visited(heroPosition) = MiniMapStatus::Visited;

    for (auto neighbor : computeFogRange(heroPosition, miniMap.visited.getSize())) {
      miniMap.visited(neighbor) = MiniMapStatus::Visited;
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
