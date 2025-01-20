#ifndef BE_GAME_STATE_H
#define BE_GAME_STATE_H

#include <cstdint>

#include <gf/Path.h>
#include <gf/Model.h>

#include "HeroState.h"
#include "MapState.h"
#include "BubbleState.h"

namespace be {

  struct GameState : gf::Model {
    static constexpr uint16_t Version = 1;

    MapState map;
    HeroState hero;
    std::vector<BubbleProducerState> producers;

    void update(gf::Time time);

    void loadFromFile(const gf::Path& filename);
    void saveToFile(const gf::Path& filename);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, GameState& state) {
    return ar | state.map | state.hero;
  }

}


#endif // BE_GAME_STATE_H
