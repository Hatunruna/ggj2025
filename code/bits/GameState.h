#ifndef BE_GAME_STATE_H
#define BE_GAME_STATE_H

#include <cstdint>

#include <array>
#include <vector>

#include <gf/Path.h>
#include <gf/Model.h>

#include "BubbleState.h"
#include "CityState.h"
#include "ContractState.h"
#include "FeedbackState.h"
#include "HeroState.h"
#include "MiniMapState.h"
#include "MapState.h"
#include "PhysicsModel.h"
#include "TrapState.h"

namespace be {

  constexpr float ContractStep = 0.20f;
  constexpr float ContractMaxStep = 1.0f;

  struct GameState : gf::Model {
    static constexpr uint16_t Version = 1;

    PhysicsModel physics;

    MapState map;
    MiniMapState miniMap;
    HeroState hero;
    std::vector<BubbleProducerState> producers;
    std::vector<BubbleState> bubbles;
    std::vector<BubbleState> freeBubbles;
    std::array<CityState, CityCount> cities;
    ContractState contract;
    FeedbackState feedback;
    float contractProgress = ContractStep;
    std::vector<TrapState> traps;



    void initializePhysics();

    void moveHero(gf::Vector2i direction);
    void teleportHero(gf::Vector2f position);
    void tryToTakeBubble();
    void releaseBubble();
    void deleteAllBubbles();
    bool tryToEnterCity();

    float computeBubblesValues();

    void update(gf::Time time);

    void loadFromFile(const gf::Path& filename);
    void saveToFile(const gf::Path& filename);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, GameState& state) {
    return ar | state.map | state.hero | state.producers | state.bubbles | state.cities | state.contract | state.contractProgress;
  }

}


#endif // BE_GAME_STATE_H
