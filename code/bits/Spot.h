#ifndef BE_SPOT_H
#define BE_SPOT_H

#include <gf/Vector.h>
#include <gf/VectorOps.h>

#include "MapSettings.h"

namespace be {

  struct Spot {
    Spot() = default;

    Spot(gf::Vector2i position)
    : position(position)
    , location((position + 0.5f) * TileSize)
    {
    }

    gf::Vector2i position = { 0, 0 };
    gf::Vector2f location = { 0.0f, 0.0f };
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, Spot& spot) {
    return ar | spot.position | spot.location;
  }

}

#endif // BE_SPOT_H
