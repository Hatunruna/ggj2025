#ifndef BE_HERO_STATE_H
#define BE_HERO_STATE_H

#include <chipmunk/chipmunk.h>

#include <gf/Orientation.h>
#include <gf/Vector.h>
#include <gf/VectorOps.h>
#include <gf/Time.h>

namespace be {

  constexpr float HeroVelocity = 200.0f;
  constexpr int HeroMaxBubble = 5;
  constexpr float HeroInvincibilityTime = 1.0f;
  constexpr int HeroMaxHealth = 5;

  struct HeroState {
    cpBody* control = nullptr;
    cpBody* body = nullptr;
    gf::Vector2f location;
    int64_t money = 0;
    int life = HeroMaxHealth;
    gf::Time lastHitTime;
    gf::Orientation orientation = gf::Orientation::Center;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, HeroState& state) {
    return ar | state.location | state.money;
  }

}

#endif // BE_HERO_STATE_H
