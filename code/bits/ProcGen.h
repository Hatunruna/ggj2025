#ifndef BE_PROCGEN_H
#define BE_PROCGEN_H

#include <gf/Random.h>

#include "GameState.h"

namespace be {

  GameState generateNewGame(gf::Random& random);

}

#endif // BE_PROCGEN_H
