#include "Namegen.h"

#include <cassert>

#include <random>
#include <set>

#include <gf/Clock.h>

namespace be {

  namespace {

    constexpr char Sentinel = '#';

  }

  /*
   * NameGeneratorModel
   */

  NamegenModel::NamegenModel(const std::vector<std::string>& data, std::size_t order, double prior, std::string alphabet)
  : m_order(order)
  , m_prior(prior)
  , m_alphabet(std::move(alphabet))
  {
    assert(0.0 <= prior && prior <= 1.0);
    train(data);
    buildChains();
  }

  std::optional<char> NamegenModel::generate(const std::string& context, gf::Random& random) const
  {
    auto iterator = m_chains.find(context);

    if (iterator == m_chains.end()) {
      return std::nullopt;
    }

    auto& weights = iterator->second;

    std::discrete_distribution<std::size_t> distribution(weights.begin(), weights.end());
    auto index = distribution(random.getEngine());
    assert(index < m_alphabet.size());
    return m_alphabet[index];
  }

  void NamegenModel::retrain(const std::vector<std::string>& data)
  {
    train(data);
    buildChains();
  }

  void NamegenModel::train(const std::vector<std::string>& data)
  {
    for (auto& item : data) {
      std::string d = std::string(m_order, Sentinel) + item + Sentinel;

      for (std::size_t i = 0; i < d.size() - m_order; ++i) {
        const std::string key = d.substr(i, m_order);
        m_observations[key].push_back(d[i + m_order]);
      }
    }
  }

  void NamegenModel::buildChains()
  {
    m_chains.clear();

    for (auto& [ context, observation ] : m_observations) {
      auto& values = m_chains[context];

      for (char prediction : m_alphabet) {
        values.push_back(m_prior + std::count(observation.begin(), observation.end(), prediction));
      }
    }
  }

  /*
   * NamegenGenerator
   */

  NamegenGenerator::NamegenGenerator(const std::vector<std::string>& data, std::size_t order, double prior, bool backoff)
  : m_order(order)
  , m_prior(prior)
  , m_backoff(backoff)
  {
    std::set<char> letters;

    for (auto& item : data) {
      for (auto c : item) {
        letters.insert(c);
      }
    }

    std::string alphabet(letters.begin(), letters.end());
    alphabet.push_back(Sentinel);

    if (backoff) {
      for (std::size_t i = 0; i < m_order; ++i) {
        m_models.emplace_back(data, order - i, prior, alphabet);
      }
    } else {
      m_models.emplace_back(data, order, prior, alphabet);
    }
  }

  std::string NamegenGenerator::generate(gf::Random& random) const
  {
    std::string word(m_order, Sentinel);

    auto maybe_letter = computeLetter(word, random);

    while (maybe_letter && maybe_letter.value() != Sentinel) {
      word.push_back(maybe_letter.value());
      maybe_letter = computeLetter(word, random);
    }

    return word;
  }

  std::optional<char> NamegenGenerator::computeLetter(const std::string& word, gf::Random& random) const
  {
    assert(word.size() >= m_order);

    std::string context = word.substr(word.size() - m_order);
    assert(context.size() == m_order);

    for (auto& model : m_models) {
      auto maybe_letter = model.generate(context, random);

      if (maybe_letter && maybe_letter.value() != Sentinel) {
        return maybe_letter.value();
      }

      context = context.substr(1);
    }

    return std::nullopt;
  }

  /*
   * NamegenManager
   */

  namespace {

    bool satisfySettings(const std::string& word, const NamegenSettings& settings)
    {
      if (word.size() < settings.minLength || word.size() > settings.maxLength) {
        return false;
      }

      if (settings.startsWith.size() > word.size() || word.substr(0, settings.startsWith.size()) != settings.startsWith) {
        return false;
      }

      if (settings.endsWith.size() > word.size() || word.substr(word.size() - settings.endsWith.size()) != settings.endsWith) {
        return false;
      }

      if (!settings.includes.empty() && word.find(settings.includes) == std::string::npos) {
        return false;
      }

      if (!settings.excludes.empty() && word.find(settings.excludes) != std::string::npos) {
        return false;
      }

      return true;
    }

  }

  NamegenManager::NamegenManager(const std::vector<std::string>& data, std::size_t order, double prior, bool backoff)
  : m_generator(data, order, prior, backoff)
  {
  }

  std::optional<std::string> NamegenManager::generateSingle(gf::Random& random, const NamegenSettings& settings)
  {
    std::string name = m_generator.generate(random);
    name.erase(std::remove(name.begin(), name.end(), Sentinel), name.end());

    if (satisfySettings(name, settings)) {
      return name;
    }

    return std::nullopt;
  }

  std::vector<std::string> NamegenManager::generateMultiple(gf::Random& random, std::size_t count, gf::Time maxTimePerName, const NamegenSettings& settings)
  {
    std::vector<std::string> names;
    gf::Clock clock;

    while (names.size() < count && clock.restart() < maxTimePerName) {
      if (auto maybe_name = generateSingle(random, settings); maybe_name) {
        names.push_back(std::move(maybe_name).value());
      }
    }

    return names;
  }

}
