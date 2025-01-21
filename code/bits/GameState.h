#ifndef BE_GAME_STATE_H
#define BE_GAME_STATE_H

#include <cstdint>

#include <array>
#include <vector>

#include <gf/Path.h>
#include <gf/Model.h>

#include "BubbleState.h"
#include "CityState.h"
#include "HeroState.h"
#include "MapState.h"
#include "PhysicsModel.h"

namespace be {

  struct GameState : gf::Model {
    static constexpr uint16_t Version = 1;

    PhysicsModel physics;

    MapState map;
    HeroState hero;
    std::vector<BubbleProducerState> producers;
    std::vector<BubbleState> bubbles;
    std::array<CityState, CityCount> cities;

    void initializePhysics();

    void moveHero(gf::Vector2i direction);
    void tryToTakeBubble();

    void update(gf::Time time);

    void loadFromFile(const gf::Path& filename);
    void saveToFile(const gf::Path& filename);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, GameState& state) {
    return ar | state.map | state.hero | state.producers | state.bubbles | state.cities;
  }

}


#endif // BE_GAME_STATE_H
