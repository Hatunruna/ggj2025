#ifndef BE_SUPPORT_H
#define BE_SUPPORT_H

#include <cstdint>

#include <algorithm>
#include <optional>
#include <vector>

#include <gf/Math.h>
#include <gf/Vector.h>

namespace be {

  class AndresAlgorithm {
  public:
    AndresAlgorithm(int32_t radius);

    std::optional<gf::Vector2i> step();

  private:
    int32_t m_radius = 0;
    int32_t m_d = 0;
    int32_t m_x = 0;
    int32_t m_y = 0;
  };

  std::vector<gf::Vector2i> generateCircle(gf::Vector2i center, int32_t radius);

  constexpr float smooth(float x, float a, float b)
  {
    const float lambda = std::min(1.0f, std::max(0.0f, (x - a) / (b - a)));
    return gf::cubicStep(lambda);
  }

}

#endif // BE_SUPPORT_H
