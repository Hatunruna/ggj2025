#ifndef BE_GAME_STATE_H
#define BE_GAME_STATE_H

#include <cstdint>

#include <gf/Path.h>

#include "MapState.h"

namespace be {

  struct GameState {
    static constexpr uint16_t Version = 1;

    MapState map;

    void loadFromFile(const gf::Path& filename);
    void saveToFile(const gf::Path& filename);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, GameState& state) {
    return ar | state.map;
  }

}


#endif // BE_GAME_STATE_H
