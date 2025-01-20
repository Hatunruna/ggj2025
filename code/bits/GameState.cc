#include "GameState.h"

#include <gf/Serialization.h>
#include <gf/SerializationOps.h>
#include <gf/Streams.h>

namespace be {

  void GameState::update(gf::Time time)
  {
    for (auto& producer : producers) {
      producer.lifetime += time;

      switch (producer.status) {
        case BubbleProducerStatus::Emerging:
          producer.size = producer.minSize;
          producer.status = BubbleProducerStatus::Growing;
          producer.lifetime = gf::Time::zero();
          break;

        case BubbleProducerStatus::Growing:
          producer.size += producer.growthRate * time.asSeconds();

          if (producer.size > producer.maxSize) {
            producer.lifetime = gf::Time::zero();
            producer.status = BubbleProducerStatus::Exploding;
          }
          break;

        case BubbleProducerStatus::Exploding:
          if (producer.lifetime > ExplosionLifetime) {
            producer.lifetime = gf::Time::zero();
            producer.status = BubbleProducerStatus::Emerging;
          }
          break;
      }
    }
  }

  void GameState::loadFromFile(const gf::Path& filename) {
    gf::FileInputStream file(filename);
    gf::CompressedInputStream compressed(file);
    gf::Deserializer ar(compressed);
    ar | *this;
  }

  void GameState::saveToFile(const gf::Path& filename) {
    gf::FileOutputStream file(filename);
    gf::CompressedOutputStream compressed(file);
    gf::Serializer ar(compressed, Version);
    ar | *this;
  }


}
