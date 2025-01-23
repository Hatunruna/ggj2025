#ifndef BE_CONTRACT_STATE_H
#define BE_CONTRACT_STATE_H

namespace be {

  struct ContractState {
    int originCity = -1;
    int targetCity = -1;
    int profitTarge = -1;
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, ContractState& state) {
    return ar | state.originCity | state.targetCity | state.profitTarge;
  }

}

#endif // BE_CONTRACT_STATE_H
