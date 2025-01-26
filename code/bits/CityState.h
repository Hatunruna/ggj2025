#ifndef BE_CITY_STATE_H
#define BE_CITY_STATE_H

#include <string>
#include <vector>

#include <gf/Direction.h>
#include <gf/Vector.h>

#include "BubbleState.h"
#include "Spot.h"

namespace be {

  constexpr std::size_t CityCount = 4;
  constexpr int CityRadius = 32;

  struct CityGateState{
    Spot spot;
    gf::Direction direction = gf::Direction::Center;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, CityGateState& state) {
    return ar | state.spot | state.direction;
  }

  struct CityState {
    std::string name;
    Spot spot;
    BubbleType type = BubbleType::None;
    gf::Vector2f center;
    std::vector<CityGateState> gates;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, CityState& state) {
    return ar | state.name | state.spot | state.gates;
  }

}

#endif // BE_CITY_STATE_H
