#ifndef BE_CITY_STATE_H
#define BE_CITY_STATE_H

#include <string>

#include <gf/Vector.h>

namespace be {

  constexpr std::size_t CityCount = 4;

  struct CityState {
    std::string name;
    gf::Vector2f location;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, CityState& state) {
    return ar | state.name | state.location;
  }

}

#endif // BE_CITY_STATE_H
