#include "ProcGen.h"

#include <gf/VectorOps.h>

#include "CityState.h"
#include "HeroState.h"
#include "MapSettings.h"
#include "MapState.h"

namespace be {

  namespace {

    // map generation settings

    constexpr float Threshold       = 0.4f;
    constexpr int SurvivalThreshold = 6;
    constexpr int BirthThreshold    = 8;
    constexpr int Iterations        = 7;

    // producers settings

    constexpr int ProducerCount = 100;
    constexpr float ProducerMinDistanceFromOther = 1000.0f;

    // cities settings

    constexpr int CityMinDistanceFromOther = 350;


    // raw

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

    bool isLargeGround(const gf::Array2D<RawCell>& raw, gf::Vector2i position) {
      if (raw(position) != RawCell::Ground) {
        return false;
      }

      int count = 0;

      for (gf::Vector2i neighbor : raw.get12NeighborsRange(position)) {
        if (raw(neighbor) == RawCell::Ground) {
          ++count;
        }
      }

      return count == 12;
    }

    gf::Array2D<MapCell, int32_t> transformRawMap(const gf::Array2D<RawCell>& raw, gf::Random& random)
    {
      gf::Array2D<MapCell, int32_t> map(MapSize);

      for (gf::Vector2i position : raw.getPositionRange()) {
        if (raw(position) == RawCell::Block) {
          map(position).type = CellType::Block;
        } else {
          map(position).type = CellType::Ground;
        }

        map(position).tile = random.computeUniformInteger(0, 3);
      }

      for (gf::Vector2i position : map.getPositionRange()) {
        if (position.y > 0 && map(position).type == CellType::Block && map({ position.x, position.y - 1 }).type == CellType::Ground) {
          map(position).type = CellType::Cliff;
        }
      }

      return map;
    }

    HeroState computeHero(const gf::Array2D<RawCell>& raw, gf::Random& random)
    {
      HeroState state = {};

      for (;;) {
        const gf::Vector2i position = random.computePosition(gf::RectI::fromSize(raw.getSize()));

        if (!isLargeGround(raw, position)) {
          continue;
        }

        state.location = (position + 0.5f) * TileSize;
        break;
      }

      return state;
    }

    bool isFarFromProducers(const std::vector<BubbleProducerState>& producers, gf::Vector2f location, float minDistance)
    {
      for (auto& producer : producers) {
        if (gf::squareDistance(producer.location, location) < gf::square(minDistance)) {
          return false;
        }
      }

      return true;
    }

    std::vector<BubbleProducerState> computeProducers(const gf::Array2D<RawCell>& raw, gf::Random& random)
    {
      std::vector<BubbleProducerState> producers;

      for (int i = 0; i < ProducerCount; ++i) {
        for (;;) {
          const gf::Vector2i position = random.computePosition(gf::RectI::fromSize(raw.getSize() - 1));

          if (!isLargeGround(raw, position)) {
            continue;
          }

          const gf::Vector2f location = (position + 0.5f) * TileSize;

          if (!isFarFromProducers(producers, location, ProducerMinDistanceFromOther)) {
            continue;
          }

          BubbleProducerState state = {};
          state.status = BubbleProducerStatus::Growing;
          state.location = location;
          state.minSize = 0.1f;
          state.maxSize = 1.0f;
          state.growthRate = 0.02f;
          state.size = random.computeUniformFloat(state.minSize, state.maxSize);

          producers.push_back(std::move(state));
          break;
        }
      }

      return producers;
    }

    std::array<CityState, CityCount> computeCities(const gf::Array2D<RawCell>& raw, gf::Random& random)
    {
      std::array<CityState, CityCount> cities;

      std::array<gf::Vector2i, CityCount> positions;

      for (;;) {
        for (std::size_t i = 0; i < CityCount; ++i) {
          positions[i] = random.computePosition(gf::RectI::fromSize(raw.getSize() - 1).shrink(32));
        }

        int minDistance = std::numeric_limits<int>::max();

        for (std::size_t i = 0; i < CityCount; ++i) {
          for (std::size_t j = i + 1; j < CityCount; ++j) {
            int distance = gf::manhattanDistance(positions[i], positions[j]);
            minDistance = std::min(minDistance, distance);
          }
        }

        if (minDistance > CityMinDistanceFromOther) {
          break;
        }
      }

      static_assert(CityCount < 26);
      std::string name = "A";

      for (std::size_t i = 0; i < CityCount; ++i) {
        cities[i].name = name;
        cities[i].location = (positions[i] + 0.5f) * TileSize;
        name[0]++;
      }

      return cities;
    }

  }

  GameState generateNewGame(gf::Random& random)
  {
    auto raw = generateRawMap(random);

    GameState state = {};
    state.map.cells = transformRawMap(raw, random);
    state.hero = computeHero(raw, random);
    state.producers = computeProducers(raw, random);
    state.cities = computeCities(raw, random);

    state.initializePhysics();

    return state;
  }

}
