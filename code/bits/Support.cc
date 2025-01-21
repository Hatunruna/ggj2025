#include "Support.h"

namespace be {

  AndresAlgorithm::AndresAlgorithm(int32_t radius)
  : m_radius(radius)
  , m_d(radius - 1)
  , m_y(radius)
  {
  }

  std::optional<gf::Vector2i> AndresAlgorithm::step()
  {
    if (m_y < m_x) {
      return std::nullopt;
    }

    gf::Vector2i result = gf::vec(m_x, m_y);

    if (m_d >= 2 * m_x) {
      m_d = m_d - 2 * m_x - 1;
      ++m_x;
    } else if (m_d < 2 * (m_radius - m_y)) {
      m_d = m_d + 2 * m_y - 1;
      --m_y;
    } else {
      m_d = m_d + 2 * (m_y - m_x + 1);
      ++m_x;
      --m_y;
    }

    return result;
  }

  std::vector<gf::Vector2i> generateCircle(gf::Vector2i center, int32_t radius)
  {
    AndresAlgorithm andres(radius);

    std::vector<gf::Vector2i> circle;

    auto plot_8_pixels = [&](gf::Vector2i point) {
      circle.emplace_back(center.x + point.x, center.y + point.y);
      circle.emplace_back(center.x + point.y, center.y + point.x);
      circle.emplace_back(center.x - point.x, center.y + point.y);
      circle.emplace_back(center.x - point.y, center.y + point.x);
      circle.emplace_back(center.x + point.x, center.y - point.y);
      circle.emplace_back(center.x + point.y, center.y - point.x);
      circle.emplace_back(center.x - point.x, center.y - point.y);
      circle.emplace_back(center.x - point.y, center.y - point.x);
    };

    // plot_8_pixels({ 0, radius });

    for (;;) {
      if (auto maybe_next = andres.step(); maybe_next) {
        plot_8_pixels(*maybe_next);
      } else {
        break;
      }
    }

    return circle;
  }

}
