#ifndef BE_HERO_STATE_H
#define BE_HERO_STATE_H

#include <gf/Vector.h>
#include <gf/VectorOps.h>

namespace be {

  struct HeroState {
    gf::Vector2f location;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, HeroState& state) {
    return ar | state.location;
  }

}

#endif // BE_HERO_STATE_H
