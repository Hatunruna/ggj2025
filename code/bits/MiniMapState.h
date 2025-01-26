#ifndef BE_MINI_MAP_STATE_H
#define BE_MINI_MAP_STATE_H

#include <cstdint>

#include <gf/Array2D.h>
#include <gf/Image.h>

namespace be {

  enum class MiniMapStatus : uint8_t {
    Unknwon,
    Visited,
  };

  struct MiniMapState {
    gf::Array2D<MiniMapStatus, int> visited;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MiniMapState& state) {
    return ar | state.visited;
  }

}

#endif // BE_MINI_MAP_STATE_H
