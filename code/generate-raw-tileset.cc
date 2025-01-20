#include <gf/Color.h>
#include <gf/Image.h>
#include <gf/Random.h>

#include "bits/MapSettings.h"

namespace {

  constexpr gf::Vector2i ExtendedTileSize = be::TileSize + be::Padding;

  constexpr gf::Color4f GroundColor = gf::Color::fromRgba32(0xAA, 0x7F, 0x55);
  constexpr float GroundChangeProbability = 0.1f;
  constexpr float GroundChangeMin = 0.2f;
  constexpr float GroundChangeMax = 0.4f;
  constexpr gf::Color4f BlockColor = gf::Color::fromRgba32(0x0C, 0x09, 0x06);
  constexpr float BlockChangeProbability = 0.05f;
  constexpr float BlockChangeMin = 0.2f;
  constexpr float BlockChangeMax = 0.8f;

  constexpr gf::Vector2i ImageSize = be::TilesetSize * be::TileSize + (be::TilesetSize - 1) * be::Margin + 2 * be::Padding;

  gf::Image computeGroundTile(gf::Random& random)
  {
    gf::Image tile(ExtendedTileSize, gf::Color::toRgba32(GroundColor));

    for (int x = 0; x < tile.getSize().width; ++x) {
      for (int y = 0; y < tile.getSize().height; ++y) {
        if (random.computeBernoulli(GroundChangeProbability)) {
          gf::Color4f color = gf::Color::darker(GroundColor, random.computeUniformFloat(GroundChangeMin, GroundChangeMax));
          tile.setPixel({ x, y }, gf::Color::toRgba32(color));
        }
      }
    }

    return tile;
  }

  gf::Image computeCliffTile(gf::Random& random)
  {
    gf::Image tile(ExtendedTileSize, gf::Color::toRgba32(gf::Color::darker(GroundColor, GroundChangeMax)));

    for (int x = 0; x < tile.getSize().width; ++x) {
      const int mid = tile.getSize().height / 2;
      const int extent = tile.getSize().height / 8;
      const int threshold = random.computeUniformInteger(mid - extent, mid + extent);

      for (int y = threshold; y < tile.getSize().height; ++y) {
        gf::Color4f color = BlockColor;

        if (random.computeBernoulli(BlockChangeProbability)) {
          gf::Color4f color = gf::Color::lighter(BlockColor, random.computeUniformFloat(BlockChangeMin, BlockChangeMax));
          tile.setPixel({ x, y }, gf::Color::toRgba32(color));
        }

        tile.setPixel({ x, y }, gf::Color::toRgba32(color));
      }
    }

    return tile;
  }

  gf::Image computeBlockTile(gf::Random& random)
  {
    gf::Image tile(ExtendedTileSize, gf::Color::toRgba32(BlockColor));

    for (int x = 0; x < tile.getSize().width; ++x) {
      for (int y = 0; y < tile.getSize().height; ++y) {
        if (random.computeBernoulli(BlockChangeProbability)) {
          gf::Color4f color = gf::Color::lighter(BlockColor, random.computeUniformFloat(BlockChangeMin, BlockChangeMax));
          tile.setPixel({ x, y }, gf::Color::toRgba32(color));
        }
      }
    }

    return tile;
  }

}


int main() {

  gf::Random random;

  gf::Image image(ImageSize, gf::Color::toRgba32(gf::Color::Transparent));

  for (int i = 0; i < be::TilesetSize.x; ++i) {
    gf::Image ground = computeGroundTile(random);

    for (int x = 0; x < ExtendedTileSize.x; ++x) {
      for (int y = 0; y < ExtendedTileSize.y; ++y) {
        image.setPixel(gf::vec(x, y) + gf::dirx(i * ExtendedTileSize.x), ground.getPixel({ x, y }));
      }
    }
  }

  for (int i = 0; i < be::TilesetSize.x; ++i) {
    gf::Image cliff = computeCliffTile(random);

    for (int x = 0; x < ExtendedTileSize.x; ++x) {
      for (int y = 0; y < ExtendedTileSize.y; ++y) {
        image.setPixel(gf::vec(x, y) + gf::dirx(i * ExtendedTileSize.x) + 1 * gf::diry(ExtendedTileSize.y), cliff.getPixel({ x, y }));
      }
    }
  }

  for (int i = 0; i < be::TilesetSize.x; ++i) {
    gf::Image block = computeBlockTile(random);

    for (int x = 0; x < ExtendedTileSize.x; ++x) {
      for (int y = 0; y < ExtendedTileSize.y; ++y) {
        image.setPixel(gf::vec(x, y) + gf::dirx(i * ExtendedTileSize.x) + 2 * gf::diry(ExtendedTileSize.y), block.getPixel({ x, y }));
      }
    }
  }

  image.saveToFile("tileset.png");
}
