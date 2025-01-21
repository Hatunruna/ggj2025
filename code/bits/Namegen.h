#ifndef BE_NAMEGEN_H
#define BE_NAMEGEN_H

#include <limits>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <gf/Random.h>
#include <gf/Time.h>

namespace be {

  class NamegenModel {
  public:
    NamegenModel(const std::vector<std::string>& data, std::size_t order, double prior, std::string alphabet);

    std::optional<char> generate(const std::string& context, gf::Random& random) const;
    void retrain(const std::vector<std::string>& data);

  private:
    void train(const std::vector<std::string>& data);
    void buildChains();

    std::size_t m_order = 1;
    double m_prior = 0.0;
    std::string m_alphabet;
    std::map<std::string, std::string> m_observations;
    std::map<std::string, std::vector<double>> m_chains;
  };

  class NamegenGenerator {
  public:
    NamegenGenerator(const std::vector<std::string>& data, std::size_t order, double prior, bool backoff);

    std::string generate(gf::Random& random) const;

  private:
    std::optional<char> computeLetter(const std::string& word, gf::Random& random) const;

    std::size_t m_order = 1;
    double m_prior = 0.0;
    bool m_backoff = true;
    std::vector<NamegenModel> m_models;

  };

  struct NamegenSettings {
    std::size_t minLength = 0;
    std::size_t maxLength = std::numeric_limits<std::size_t>::max();
    std::string startsWith;
    std::string endsWith;
    std::string includes;
    std::string excludes;
  };

  class NamegenManager {
  public:
    NamegenManager(const std::vector<std::string>& data, std::size_t order, double prior, bool backoff);

    std::optional<std::string> generateSingle(gf::Random& random, const NamegenSettings& settings = {});
    std::vector<std::string> generateMultiple(gf::Random& random, std::size_t count, gf::Time maxTimePerName, const NamegenSettings& settings = {});

  private:
    NamegenGenerator m_generator;
  };


}

#endif // BE_NAMEGEN_H
