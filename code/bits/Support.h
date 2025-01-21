#ifndef BE_SUPPORT_H
#define BE_SUPPORT_H

#include <cstdint>

#include <optional>
#include <vector>

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

}

#endif // BE_SUPPORT_H
