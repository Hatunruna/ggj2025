#include "ProcGen.h"

#include <cassert>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <limits>
#include <queue>

#include <gf/Log.h>
#include <gf/VectorOps.h>
#include <utility>

#include "CityState.h"
#include "ContractState.h"
#include "HeroState.h"
#include "MapSettings.h"
#include "MapState.h"
#include "Namegen.h"
#include "Support.h"

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
      Gate,
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

    constexpr std::size_t NoZone = std::numeric_limits<std::size_t>::max();

    std::size_t traverseMap(const gf::Array2D<RawCell>& raw, gf::Array2D<std::size_t, int>& zones, gf::Vector2i start, std::size_t zone)
    {
      std::queue<gf::Vector2i> queue;
      queue.push(start);
      std::size_t count = 0;

      zones(start) = zone;

      while (!queue.empty()) {
        gf::Vector2i current = queue.front();
        queue.pop();
        ++count;

        assert(raw(current) == RawCell::Ground || raw(current) == RawCell::Gate);
        assert(zones(current) == zone);

        for (auto neighbor : raw.get4NeighborsRange(current)) {
          if (raw(neighbor) == RawCell::Block) {
            continue;
          }

          if (zones(neighbor) == NoZone) {
            zones(neighbor) = zone;
            queue.push(neighbor);
          }
        }
      }

      return count;
    }

    void sanitizeMap(gf::Array2D<RawCell>& raw)
    {
      gf::Array2D<std::size_t, int> zones(raw.getSize(), NoZone);
      std::vector<std::size_t> counts;

      for (auto position : raw.getPositionRange()) {
        if (raw(position) == RawCell::Ground && zones(position) == NoZone) {
          auto count = traverseMap(raw, zones, position, counts.size());
          counts.push_back(count);
        }
      }

      auto iterator = std::max_element(counts.begin(), counts.end());
      assert(iterator != counts.end());

      const std::size_t good = std::distance(counts.begin(), iterator);

      for (auto position : raw.getPositionRange()) {
        if (zones(position) != good) {
          raw(position) = RawCell::Block;
        }
      }
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

    std::pair<ContractState, HeroState> computeStartPoint(const gf::Array2D<RawCell>& raw, const std::array<CityState, CityCount>& cities, gf::Random& random)
    {
      ContractState contract = {};
      HeroState hero = {};

      for (;;) {
        contract.originCity = random.computeUniformInteger(std::size_t(0), CityCount - 1);

        for (const auto& gateWorldPosition: cities[contract.originCity].gates) {
          const gf::Vector2i gateTilePosition = gateWorldPosition / TileSize;
          if (raw(gateTilePosition) == RawCell::Gate) {
            for (const auto position: raw.get8NeighborsRange(gateTilePosition)) {
            if (raw(position) == RawCell::Ground) {
                gf::Log::debug("Origin City: %s\n", cities[contract.originCity].name.c_str());
                hero.location = (position + 0.5f) * TileSize;
                return std::make_pair(contract, hero);
              }
            }
          }
        }
      }
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

    std::array<CityState, CityCount> computeCities(gf::Array2D<RawCell>& raw, gf::Random& random, gf::ResourceManager& resources)
    {
      /*
       * Origin: https://www.data.gouv.fr/fr/datasets/villes-de-france/#/resources
       * Sanitized to have unique names without numeric suffixes (PLM)
       */
      std::filesystem::path filename = resources.getAbsolutePath("cities.txt");
      std::ifstream file(filename);

      std::vector<std::string> data;

      for (std::string line; std::getline(file, line); ) {
        if (line.empty() || line == "\n") {
          continue;
        }

        data.push_back(std::move(line));
      }

      NamegenSettings settings = {};
      settings.minLength = 3;
      settings.maxLength = 15;

      NamegenManager namegen(data, 3, 0.001, true);
      auto names = namegen.generateMultiple(random, CityCount, gf::seconds(0.5), settings);

      for (auto& name : names) {
        gf::Log::debug("- %s\n", name.c_str());
      }

      std::array<CityState, CityCount> cities;
      std::array<gf::Vector2i, CityCount> positions;

      for (;;) {
        for (std::size_t i = 0; i < CityCount; ++i) {
          positions[i] = random.computePosition(gf::RectI::fromSize(raw.getSize() - 1).shrink(CityRadius));
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

      for (std::size_t i = 0; i < CityCount; ++i) {
        cities[i].name = names[i];
        cities[i].location = (positions[i] + 0.5f) * TileSize;

        std::vector<gf::Vector2i> points;
        points.push_back(positions[i]);

        for (int radius = 1; radius <= CityRadius; ++radius) {
          auto circle = generateCircle(positions[i], radius);
          points.insert(points.end(), circle.begin(), circle.end());
        }

        for (auto point : points) {
          raw(point) = RawCell::Block;
        }

        // Set gates
        auto setGate = [&](const auto& position, int gateIndex) -> void {
          raw(position) = RawCell::Gate;
          cities[i].gates[gateIndex] = (position + 0.5f) * TileSize; // West
        };

        setGate(positions[i] + gf::vec(-CityRadius, 0), 0); // West
        setGate(positions[i] + gf::vec(0,  CityRadius), 1); // South
        setGate(positions[i] + gf::vec( CityRadius, 0), 2); // East
      }

      return cities;
    }

  }

  GameState generateNewGame(gf::Random& random, gf::ResourceManager& resources)
  {
    gf::Log::debug("Generate raw map\n");
    auto raw = generateRawMap(random);

    GameState state = {};
    gf::Log::debug("Compute cities\n");
    state.cities = computeCities(raw, random, resources);
    gf::Log::debug("Sanitize raw map\n");
    sanitizeMap(raw);

    gf::Log::debug("Transform raw map\n");
    state.map.cells = transformRawMap(raw, random);
    gf::Log::debug("Compute start point\n");
    std::tie(state.contract, state.hero) = computeStartPoint(raw, state.cities, random);
    gf::Log::debug("Compute producers\n");
    state.producers = computeProducers(raw, random);

    gf::Log::debug("Initialize physics\n");
    state.initializePhysics();

    return state;
  }

}
