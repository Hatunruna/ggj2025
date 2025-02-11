#ifndef BE_BUBBLE_STATE_H
#define BE_BUBBLE_STATE_H

#include <cstdint>

#include <chipmunk/chipmunk.h>

#include <gf/Vector.h>
#include <gf/VectorOps.h>
#include <gf/Random.h>
#include <gf/Time.h>

#include "Spot.h"

namespace be {

  enum class BubbleProducerStatus : uint8_t {
    Emerging,
    Growing,
    Exploding,
  };

  enum class BubbleType : uint8_t {
    Red,
    Blue,
    Green,
    Yellow,

    None,
  };

  constexpr gf::Time ExplosionLifetime = gf::seconds(2.0);
  constexpr gf::Time ProducerCooldown = gf::seconds(60.0f);
  constexpr float BubbleMaxSize = 0.5f;
  constexpr float BubbleMinSize = 0.1f;
  constexpr float ProducerGrowRate = 0.02f;

  struct BubbleProducerState {
    BubbleProducerStatus status = BubbleProducerStatus::Emerging;
    Spot spot;

    // bubble
    float size = 0.0f;
    gf::Time lifetime;
    BubbleType type = BubbleType::None;

    // settings
    float minSize = 0.0f;
    float maxSize = 0.0f;
    float growthRate = 0.0f;
    int tile = 0;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, BubbleProducerState& state) {
    return ar | state.status | state.spot | state.size | state.lifetime | state.type | state.minSize | state.maxSize | state.growthRate;
  }

  struct BubbleState {
    float size = 0.0f;
    BubbleType type = BubbleType::None;
    gf::Time time = gf::Time::zero();
    cpBody* body = nullptr;
    cpShape* shape = nullptr;
    cpConstraint* pin = nullptr;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, BubbleState& state) {
    return ar | state.size | state.type;
  }

}

#endif // BE_BUBBLE_STATE_H
