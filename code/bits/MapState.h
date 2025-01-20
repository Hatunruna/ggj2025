#ifndef BE_MAP_STATE_H
#define BE_MAP_STATE_H

#include <cstdint>

#include <gf/Array2D.h>

namespace be {

  enum class CellType : uint8_t {
    Ground,
    Block,
    Cliff,
  };

  struct MapCell {
    CellType type;
    int tile = 0;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MapCell& cell) {
    return ar | cell.type;
  }

  struct MapState {
    gf::Array2D<MapCell, int32_t> cells;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, MapState& state) {
    return ar | state.cells;
  }

}

#endif // BE_MAP_STATE_H
