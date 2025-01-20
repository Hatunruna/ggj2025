#include "ProcGen.h"

#include "MapSettings.h"
#include "MapState.h"

namespace be {

  namespace {

    constexpr float Threshold       = 0.4f;
    constexpr int SurvivalThreshold = 6;
    constexpr int BirthThreshold    = 8;
    constexpr int Iterations        = 7;


    enum class RawCell {
      Ground,
      Block,
      Limit,
    };

    gf::Array2D<RawCell> generateRawMap(gf::Random& random)
    {
      gf::Array2D<RawCell> raw(MapSize);

      for (auto& cell : raw) {
        if (random.computeUniformFloat(0.0f, 1.0f) > Threshold) {
          cell = RawCell::Ground;
        } else {
          cell = RawCell::Block;
        }
      }

      for (int i = 0; i < Iterations; ++i) {
        gf::Array2D<RawCell> next(MapSize);

        for (const gf::Vector2i position : raw.getPositionRange()) {
          int count = 0;

          for (const gf::Vector2i neighbor : raw.get12NeighborsRange(position)) {
            count += raw(neighbor) == RawCell::Ground ? 1 : 0;;
          }

          if (raw(position) == RawCell::Ground) {
            if (count >= SurvivalThreshold) {
              next(position) = RawCell::Ground;
            } else {
              next(position) = RawCell::Block;
            }
          } else {
            if (count >= BirthThreshold) {
              next(position) = RawCell::Ground;
            } else {
              next(position) = RawCell::Block;
            }
          }
        }

        raw = std::move(next);
      }

      return raw;
    }


    gf::Array2D<MapCell, int32_t> transformRawMap(const gf::Array2D<RawCell>& raw)
    {
      gf::Array2D<MapCell, int32_t> map(MapSize);

      for (gf::Vector2i position : raw.getPositionRange()) {
        if (raw(position) == RawCell::Block) {
          map(position).type = CellType::Block;
        } else {
          map(position).type = CellType::Ground;
        }
      }

      return map;
    }

    void sanitizeMap(gf::Array2D<MapCell, int32_t>& map) {

      for (gf::Vector2i position : map.getPositionRange()) {
        if (position.y > 0 && map(position).type == CellType::Block && map({ position.x, position.y - 1 }).type == CellType::Ground) {
          map(position).type = CellType::Cliff;
        }
      }

    }

  }





  GameState generateNewGame(gf::Random& random)
  {
    auto raw = generateRawMap(random);


    GameState state = {};

    state.map.cells = transformRawMap(raw);
    sanitizeMap(state.map.cells);

    return state;
  }

}
