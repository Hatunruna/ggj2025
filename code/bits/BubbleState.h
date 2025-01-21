#ifndef BE_BUBBLE_STATE_H
#define BE_BUBBLE_STATE_H

#include <cstdint>

#include <chipmunk/chipmunk.h>

#include <gf/Vector.h>
#include <gf/VectorOps.h>
#include <gf/Time.h>

namespace be {

  enum class BubbleProducerStatus : uint8_t {
    Emerging,
    Growing,
    Exploding,
  };

  constexpr gf::Time ExplosionLifetime = gf::seconds(2.0);

  struct BubbleProducerState {
    BubbleProducerStatus status = BubbleProducerStatus::Emerging;
    gf::Vector2f location;

    // bubble
    float size = 0.0f;
    gf::Time lifetime;

    // settings
    float minSize = 0.0f;
    float maxSize = 0.0f;
    float growthRate = 0.0f;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, BubbleProducerState& state) {
    return ar | state.status | state.location | state.size | state.lifetime | state.minSize | state.maxSize | state.growthRate;
  }

  struct BubbleState {
    float size = 0.0f;
    cpBody* body = nullptr;
    cpConstraint* pin = nullptr;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, BubbleState& state) {
    return ar | state.size;
  }

}

#endif // BE_BUBBLE_STATE_H
