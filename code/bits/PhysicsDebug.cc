#include "PhysicsDebug.h"

#include <chipmunk/chipmunk.h>


#include "GameHub.h"
#include "gf/Vector.h"

namespace be {

  namespace {

    constexpr int DebugPointCount = 20;
    constexpr float DebugLineThickness = 1.0f;
    constexpr float DebugAlpha = 0.8f;


    gf::Vector2f toVec(cpVect v)
    {
      return gf::vec(v.x, v.y);
    }

    gf::Color4f toColor(cpSpaceDebugColor color)
    {
      return { color.r, color.g, color.b, DebugAlpha };
    }

    gf::Polygon lineToPolygon(gf::Vector2f a, gf::Vector2f b, float thickness)
    {
      const gf::Vector2f normal = gf::normalize(gf::perp(b - a));

      gf::Polygon polygon;

      polygon.addPoint(a + 0.5f * thickness * normal);
      polygon.addPoint(a - 0.5f * thickness * normal);
      polygon.addPoint(b - 0.5f * thickness * normal);
      polygon.addPoint(b + 0.5f * thickness * normal);

      return polygon;
    }

    void spaceDebugDrawCircle(cpVect pos, cpFloat angle, cpFloat radius, cpSpaceDebugColor outlineColor, cpSpaceDebugColor fillColor, cpDataPointer data)
    {
      auto particles = static_cast<gf::ShapeParticles*>(data);

      particles->addCircle(toVec(pos), static_cast<float>(radius) + DebugLineThickness, toColor(outlineColor), DebugPointCount);
      particles->addCircle(toVec(pos), static_cast<float>(radius), toColor(fillColor), DebugPointCount);

      gf::Polygon polygon = lineToPolygon(toVec(pos), toVec(pos) + radius * gf::unit(angle), DebugLineThickness);
      particles->addPolygon(polygon, toColor(outlineColor));
    }

    void spaceDebugDrawSegment(cpVect a, cpVect b, cpSpaceDebugColor color, cpDataPointer data)
    {
      auto particles = static_cast<gf::ShapeParticles*>(data);

      gf::Polygon polygon = lineToPolygon(toVec(a), toVec(b), DebugLineThickness);
      particles->addPolygon(polygon, toColor(color));
    }

    void spaceDebugDrawFatSegment(cpVect a, cpVect b, cpFloat radius, cpSpaceDebugColor outlineColor, cpSpaceDebugColor fillColor, cpDataPointer data)
    {
      auto particles = static_cast<gf::ShapeParticles*>(data);

      gf::Polygon outlinePolygon = lineToPolygon(toVec(a), toVec(b), 2 * radius + DebugLineThickness);
      particles->addPolygon(outlinePolygon, toColor(outlineColor));

      gf::Polygon fillPolygon = lineToPolygon(toVec(a), toVec(b), 2 * radius);
      particles->addPolygon(fillPolygon, toColor(fillColor));
    }

    void spaceDebugDrawPolygon(int count, const cpVect *verts, [[maybe_unused]] cpFloat radius, [[maybe_unused]] cpSpaceDebugColor outlineColor, cpSpaceDebugColor fillColor, cpDataPointer data)
    {
      auto particles = static_cast<gf::ShapeParticles*>(data);

      gf::Polygon polygon;

      for (int i = 0; i < count; ++i) {
        polygon.addPoint(toVec(verts[i]));
      }

      // polygon.getCenter();

      particles->addPolygon(polygon, toColor(fillColor));
    }

    void spaceDebugDrawDot(cpFloat size, cpVect pos, cpSpaceDebugColor color, cpDataPointer data)
    {
      auto particles = static_cast<gf::ShapeParticles*>(data);

      particles->addCircle(toVec(pos), static_cast<float>(size), toColor(color), DebugPointCount);
    }

    cpSpaceDebugColor spaceDebugDrawColorForShape([[maybe_unused]] cpShape *shape, [[maybe_unused]] cpDataPointer data)
    {
      return { 0.5f, 0.5f, 0.5f, DebugAlpha };
    }

  }

  PhysicsDebug::PhysicsDebug(GameHub& game)
  : m_game(game)
  {
  }

  void PhysicsDebug::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    if (!m_debug) {
      return;
    }

    m_particles.clear();

    cpSpaceDebugDrawOptions options;
    options.drawCircle = spaceDebugDrawCircle;
    options.drawSegment = spaceDebugDrawSegment;
    options.drawFatSegment = spaceDebugDrawFatSegment;
    options.drawPolygon = spaceDebugDrawPolygon;
    options.drawDot = spaceDebugDrawDot;
    options.flags = static_cast<cpSpaceDebugDrawFlags>(CP_SPACE_DEBUG_DRAW_SHAPES | CP_SPACE_DEBUG_DRAW_CONSTRAINTS | CP_SPACE_DEBUG_DRAW_COLLISION_POINTS);
    options.shapeOutlineColor = { 1.0f, 0.0f, 0.0f, 0.5f };
    options.colorForShape = spaceDebugDrawColorForShape;
    options.constraintColor = { 0.0f, 1.0f, 0.0f, 0.5f };
    options.collisionPointColor = { 0.0f, 0.0f, 1.0f, 0.5f };
    options.data = &m_particles;
    cpSpaceDebugDraw(m_game.state.physics.getSpace(), &options);

    target.draw(m_particles, states);
  }

}
