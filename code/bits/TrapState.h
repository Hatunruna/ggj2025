#ifndef BE_TRAP_STATE_H
#define BE_TRAP_STATE_H

#include <gf/Clock.h>

#include "MapSettings.h"
#include "Spot.h"

namespace be {

  constexpr float TrapTriggerTime = 1.5f;
  constexpr float TrapActiveTime = TrapTriggerTime + 8.0f;
  constexpr float TrapRestTime = TrapActiveTime + 2.0f;

  constexpr float TrapTriggerDistance = TileSize.x * 3.0f;
  constexpr float TrapGrowRate = TileSize.x * 4; // 4 tile/sec
  constexpr float TrapMaxHitRadius = TileSize.x * 4.0f;

  enum class TrapStatus {
    Wait,     // Ready to spot the hero
    Trigger,  // The hero is to near of trap
    Active,   // Can harm the hero
    Rest,     // Cooldown between to attacks
  };

  struct TrapState {
    Spot spot;
    TrapStatus status;
    float radius = 0.0f;
    gf::Clock clock;
  };

}

#endif // BE_TRAP_STATE_H
