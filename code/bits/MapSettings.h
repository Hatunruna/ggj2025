#ifndef BE_MAP_SETTINGS_H
#define BE_MAP_SETTINGS_H

#include <gf/Vector.h>

namespace be {

  constexpr gf::Vector2i MapSize = { 512, 512 };

  constexpr gf::Vector2i TilesetSize = { 4, 4 };
  constexpr gf::Vector2i TileSize = { 32, 32 };
  constexpr int Margin = 1;
  constexpr int Padding = 2;

}

#endif // BE_MAP_SETTINGS_H
