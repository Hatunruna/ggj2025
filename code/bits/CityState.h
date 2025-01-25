#ifndef BE_CITY_STATE_H
#define BE_CITY_STATE_H

#include <string>
#include <vector>

#include <gf/Vector.h>

#include "Spot.h"

namespace be {

  constexpr std::size_t CityCount = 4;
  constexpr int CityRadius = 32;

  struct CityState {
    std::string name;
    Spot spot;
    std::vector<Spot> gates;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, CityState& state) {
    return ar | state.name | state.spot | state.gates;
  }

}

#endif // BE_CITY_STATE_H
