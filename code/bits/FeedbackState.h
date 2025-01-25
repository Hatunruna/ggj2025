#ifndef BE_FEEDBACK_STATE_H
#define BE_FEEDBACK_STATE_H

#include <string>

#include <gf/Clock.h>

namespace be {

  constexpr float FeedbackTime = 2.0f;
  constexpr float FeedbackFadeTime = 0.5f;

  struct FeedbackState {
    std::string message;
    gf::Clock time;
  };

}

#endif // BE_FEEDBACK_STATE_H
