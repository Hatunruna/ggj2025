#ifndef be_GAME_STATE_H
#define be_GAME_STATE_H

#include <cstdint>

#include <gf/Path.h>

namespace be {

  struct GameState {
    static constexpr uint16_t Version = 1;

    void loadFromFile(const gf::Path& filename);
    void saveToFile(const gf::Path& filename);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, GameState& state) {
    return ar;
  }

}


#endif // be_GAME_STATE_H
