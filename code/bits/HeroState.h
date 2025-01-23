#ifndef BE_HERO_STATE_H
#define BE_HERO_STATE_H

#include <chipmunk/chipmunk.h>

#include <gf/Vector.h>
#include <gf/VectorOps.h>

namespace be {

  constexpr float HeroVelocity = 200.0f;

  struct HeroState {
    cpBody* control = nullptr;
    cpBody* body = nullptr;
    gf::Vector2f location;
    int64_t money = 0;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, HeroState& state) {
    return ar | state.location | state.money;
  }

}

#endif // BE_HERO_STATE_H
