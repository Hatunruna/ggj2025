#ifndef BE_CONTRACT_STATE_H
#define BE_CONTRACT_STATE_H

#include "BubbleState.h"
#include "HeroState.h"

namespace be {

  constexpr float BubbleBaseValue = 100.0f;
  constexpr float MaxContract = BubbleMaxSize * BubbleBaseValue * HeroMaxBubble * 0.75f;

  struct ContractState {
    int originCity = -1;
    int targetCity = -1;
    int bubbleValueTarget = -1;
    BubbleType type = BubbleType::None;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, ContractState& state) {
    return ar | state.originCity | state.targetCity | state.bubbleValueTarget;
  }

}

#endif // BE_CONTRACT_STATE_H
