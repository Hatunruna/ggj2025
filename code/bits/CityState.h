#ifndef BE_CITY_STATE_H
#define BE_CITY_STATE_H

#include <array>
#include <string>

#include <gf/Vector.h>

namespace be {

  constexpr std::size_t CityCount = 4;
  constexpr int CityRadius = 32;
  constexpr int CityGateCount = 3;

  struct CityState {
    std::string name;
    gf::Vector2f location;
    std::array<gf::Vector2f, CityGateCount> gates;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, CityState& state) {
    return ar | state.name | state.location | state.gates;
  }

}

#endif // BE_CITY_STATE_H
