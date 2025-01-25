#include "ProcGen.h"

#include <cassert>

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <limits>
#include <queue>
#include <utility>

#include <gf/Direction.h>
#include <gf/Log.h>
#include <gf/VectorOps.h>

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
    constexpr int ProducerMinDistanceFromOther = 32;

    // cities settings

    constexpr int CityMinDistanceFromOther = 350;

    // raw

    class Sanitizer {
    public:
      static constexpr std::size_t NoZone = std::numeric_limits<std::size_t>::max();

      Sanitizer(gf::Array2D<MapCell, int32_t>& cells)
      : m_cells(cells)
      , m_zones(cells.getSize(), NoZone)
      {
      }

      void run()
      {
        std::size_t chosen_zone = NoZone;
        std::size_t chosen_count = 0;

        std::size_t current_zone = 0;

        for (auto position : m_cells.getPositionRange()) {
          if (m_cells(position).type == CellType::Ground && m_zones(position) == NoZone) {
            auto count = traverse(position, current_zone);

            if (count > chosen_count) {
              chosen_count = count;
              chosen_zone = current_zone;
            }

            ++current_zone;
          }
        }

        assert(chosen_zone != NoZone);

        for (auto position : m_cells.getPositionRange()) {
          if (m_zones(position) != chosen_zone) {
            m_cells(position).type = CellType::Block;
          }
        }
      }

    private:
      std::size_t traverse(gf::Vector2i start, std::size_t zone)
      {
        std::queue<gf::Vector2i> queue;
        queue.push(start);
        std::size_t count = 0;

        m_zones(start) = zone;

        while (!queue.empty()) {
          gf::Vector2i current = queue.front();
          queue.pop();
          ++count;

          assert(m_cells(current).type == CellType::Ground);
          assert(m_zones(current) == zone);

          for (auto neighbor : m_cells.get4NeighborsRange(current)) {
            if (m_cells(neighbor).type == CellType::Block) {
              continue;
            }

            if (m_zones(neighbor) == NoZone) {
              m_zones(neighbor) = zone;
              queue.push(neighbor);
            }
          }
        }

        return count;
      }

      gf::Array2D<MapCell, int32_t>& m_cells;
      gf::Array2D<std::size_t, int> m_zones;
    };


    class Generator {
    public:
      Generator(GameState& state, gf::Random& random, gf::ResourceManager& resources)
      : m_state(state)
      , m_random(random)
      {
        /*
         * Origin: https://www.data.gouv.fr/fr/datasets/villes-de-france/#/resources
         * Sanitized to have unique names without numeric suffixes (PLM)
         */
        std::filesystem::path filename = resources.getAbsolutePath("cities.txt");
        std::ifstream file(filename);

        for (std::string line; std::getline(file, line); ) {
          if (line.empty() || line == "\n") {
            continue;
          }

          m_data.push_back(std::move(line));
        }
      }

      void run()
      {
        generateBaseMap();
        generateCities();
        sanitizeMap();
        generatePaths();

        computeStartPoint();
        computeProducers();
        computeTiles();

      }

    private:
      void generateBaseMap()
      {
        auto& cells = m_state.map.cells;
        cells = gf::Array2D<MapCell, int32_t>(MapSize);

        for (auto& cell : cells) {
          if (m_random.computeUniformFloat(0.0f, 1.0f) > Threshold) {
            cell.type = CellType::Ground;
          } else {
            cell.type = CellType::Block;
          }
        }

        for (int i = 0; i < Iterations; ++i) {
          gf::Array2D<MapCell, int32_t> next(MapSize);

          for (const gf::Vector2i position : cells.getPositionRange()) {
            int count = 0;

            for (const gf::Vector2i neighbor : cells.get12NeighborsRange(position)) {
              count += cells(neighbor).type == CellType::Ground ? 1 : 0;;
            }

            if (cells(position).type == CellType::Ground) {
              if (count >= SurvivalThreshold) {
                next(position).type = CellType::Ground;
              } else {
                next(position).type = CellType::Block;
              }
            } else {
              if (count >= BirthThreshold) {
                next(position).type = CellType::Ground;
              } else {
                next(position).type = CellType::Block;
              }
            }
          }

          cells = std::move(next);
        }
      }

      void generateCities()
      {
        // positions

        std::array<gf::Vector2i, CityCount> positions;

        for (;;) {
          for (std::size_t i = 0; i < CityCount; ++i) {
            positions[i] = m_random.computePosition(gf::RectI::fromSize(MapSize - 1).shrink(CityRadius));
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

        // names

        NamegenSettings settings = {};
        settings.minLength = 5;
        settings.maxLength = 12;

        NamegenManager namegen(m_data, 3, 0.001, true);
        auto names = namegen.generateMultiple(m_random, CityCount, gf::seconds(0.5), settings);

        for (auto& name : names) {
          gf::Log::debug("- %s\n", name.c_str());
        }

        // cities

        auto& cells = m_state.map.cells;

        for (std::size_t i = 0; i < CityCount; ++i) {
          m_state.cities[i].name = names[i];
          m_state.cities[i].spot = positions[i];

          std::vector<gf::Vector2i> points;
          points.push_back(positions[i]);

          for (int radius = 1; radius <= CityRadius; ++radius) {
            auto circle = generateCircle(positions[i], radius);
            points.insert(points.end(), circle.begin(), circle.end());
          }

          for (auto point : points) {
            cells(point).type = CellType::Block;
          }
        }

        // gates

        for (auto& city : m_state.cities) {
          for (auto direction : { gf::Direction::Left, gf::Direction::Down, gf::Direction::Right }) {
            gf::Vector2i current = city.spot.position;

            for (;;) {
              if (!cells.isValid(current)) {
                break;
              }

              if (gf::manhattanDistance(current, city.spot.position) > CityRadius + 5) {
                break;
              }

              if (cells(current).type == CellType::Ground) {
                city.gates.push_back(current);
                break;
              }

              current += gf::displacement(direction);
            }
          }
        }
      }

      void sanitizeMap()
      {
        Sanitizer sanitizer(m_state.map.cells);
        sanitizer.run();
      }

      void generatePaths()
      {
        // TODO
      }

      void computeStartPoint()
      {
        const std::size_t city = m_random.computeUniformInteger(std::size_t(0), CityCount - 1);
        m_state.contract.originCity = m_state.contract.targetCity = city;

        assert(!m_state.cities[city].gates.empty());
        const std::size_t gate = m_random.computeUniformInteger(std::size_t(0), m_state.cities[city].gates.size() - 1);

        m_state.hero.location = m_state.cities[city].gates[gate].location;
      }

      bool isLargeGround(gf::Vector2i position) {
        auto& cells = m_state.map.cells;

        if (cells(position).type != CellType::Ground) {
          return false;
        }

        int count = 0;

        for (gf::Vector2i neighbor : cells.get24NeighborsRange(position)) {
          if (cells(neighbor).type == CellType::Ground) {
            ++count;
          }
        }

        return count == 24;
      }

      bool isFarFromProducers(gf::Vector2i position, int minDistance)
      {
        for (auto& producer : m_state.producers) {
          if (gf::squareDistance(producer.spot.position, position) < gf::square(minDistance)) {
            return false;
          }
        }

        return true;
      }

      void computeProducers()
      {
        std::array<BubbleType, CityCount> types = { BubbleType::Red, BubbleType::Blue, BubbleType::Green, BubbleType::Yellow };
        std::shuffle(types.begin(), types.end(), m_random.getEngine());

        for (int i = 0; i < ProducerCount; ++i) {
          for (;;) {
            const gf::Vector2i position = m_random.computePosition(gf::RectI::fromSize(MapSize - 1));

            if (!isLargeGround(position)) {
              continue;
            }

            if (!isFarFromProducers(position, ProducerMinDistanceFromOther)) {
              continue;
            }

            BubbleProducerState producer = {};
            producer.status = BubbleProducerStatus::Growing;
            producer.spot = position;
            producer.minSize = BubbleMinSize;
            producer.maxSize = BubbleMaxSize;
            producer.growthRate = ProducerGrowRate;
            producer.size = m_random.computeUniformFloat(producer.minSize, producer.maxSize);
            producer.tile = m_random.computeUniformInteger(0, 2);

            auto iterator = std::min_element(m_state.cities.begin(), m_state.cities.end(), [&](const CityState& lhs, const CityState& rhs) {
              return gf::manhattanDistance(lhs.spot.position, position) < gf::manhattanDistance(rhs.spot.position, position);
            });

            producer.type = types[std::distance(m_state.cities.begin(), iterator)];

            m_state.producers.push_back(std::move(producer));
            break;
          }
        }
      }

      void computeTiles()
      {
        auto& cells = m_state.map.cells;

        for (gf::Vector2i position : cells.getPositionRange()) {
          if (position.y > 0 && cells(position).type == CellType::Block && cells({ position.x, position.y + 1 }).type == CellType::Ground) {
            cells(position).type = CellType::Cliff;

            if (m_random.computeBernoulli(0.95)) {
              cells(position).tile = 0;
            } else {
              cells(position).tile = m_random.computeUniformInteger(1, 6);
            }
          } else if (cells(position).type == CellType::Ground) {

            if (m_random.computeBernoulli(0.95)) {
              cells(position).tile = 0;
            } else {
              cells(position).tile = m_random.computeUniformInteger(1, 18);
            }
          }
        }
      }

      GameState& m_state;
      gf::Random& m_random;
      std::vector<std::string> m_data;
    };

  }

  GameState generateNewGame(gf::Random& random, gf::ResourceManager& resources)
  {
    gf::Log::debug("Generate raw map\n");

    GameState state = {};

    Generator generator(state, random, resources);
    generator.run();

    gf::Log::debug("Initialize physics\n");
    state.initializePhysics();

    return state;
  }

}
