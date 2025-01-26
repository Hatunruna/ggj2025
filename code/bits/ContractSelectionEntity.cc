#include "ContractSelectionEntity.h"

#include <cassert>
#include <cstdint>

#include <gf/AnimatedSprite.h>
#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/StringUtils.h>
#include <gf/Text.h>
#include <gf/Sprite.h>

#include "ContractState.h"
#include "GameHub.h"

namespace be {

  ContractSelectionEntity::ContractSelectionEntity(GameHub& game)
  : m_game(game)
  , m_font(game.resources.getFont("DejaVuSans.ttf"))
  , m_textures({
    game.resources.getTexture("animation/bubble_red.png"),
    game.resources.getTexture("animation/bubble_blue.png"),
    game.resources.getTexture("animation/bubble_green.png"),
    game.resources.getTexture("animation/bubble_yellow.png"),
    })
  , m_animations{ 
    m_redBubbleAnimation,
    m_blueBubbleAnimation,
    m_greenBubbleAnimation,
    m_yellowBubbleAnimation
    }
  , m_text1("Choice1", m_font)
  , m_text2("Choice2", m_font)
  , m_text3("Choice3", m_font)
  , m_choice1("Choice 1", m_font)
  , m_choice2("Choice 2", m_font)
  , m_choice3("Choice 3", m_font)
  {
    auto setupButton = [&] (gf::TextButtonWidget& button, auto callback) {
      button.setDefaultTextColor(gf::Color::Black);
      button.setDefaultBackgroundColor(gf::Color::Gray(0.7f));
      button.setSelectedTextColor(gf::Color::Black);
      button.setSelectedBackgroundColor(gf::Color::Green);
      button.setDisabledTextColor(gf::Color::Black);
      button.setDisabledBackgroundColor(gf::Color::Red);
      button.setAlignment(gf::Alignment::Center);
      button.setCallback(callback);
      m_widgets.addWidget(button);
    };

    setupButton(m_choice1, [&] () {
      gf::Log::debug("Choice 1\n");
      m_game.contract.selectNextContract(0);
    });

    setupButton(m_choice2, [&] () {
      gf::Log::debug("Choice 2\n");
      m_game.contract.selectNextContract(1);
    });

    setupButton(m_choice3, [&] () {
      gf::Log::debug("Choice 3\n");
      m_game.contract.selectNextContract(2);
    });

    for (std::size_t i = 0; i < CityCount; ++i) {
      m_animations[i].get().addTileset(m_textures[i], gf::vec(8, 8), gf::seconds(1.0f / 25.0f), 64);
    }
  }

  void ContractSelectionEntity::update(gf::Time time) {
    for (std::size_t i = 0; i < CityCount; ++i) {
      m_animations[i].get().update(time);
    }
  }

  void ContractSelectionEntity::updateContracts(const std::array<ContractState, CityCount - 1>& nextContracts)
  {
    const std::array<gf::TextButtonWidget*, 3> buttons = {
      &m_choice1,
      &m_choice2,
      &m_choice3,
    };

    const std::array<gf::Text*, 3> texts = {
      &m_text1,
      &m_text2,
      &m_text3,
    };

    for (std::size_t i = 0; i < nextContracts.size(); ++i) {
      const auto& city = m_game.state.cities[nextContracts[i].targetCity];
      const float distance = gf::euclideanDistance(m_game.state.hero.location, city.spot.location);
      texts[i]->setString(
        city.name + "\n\n" +
        "distance: " + gf::niceNum(distance, 1.0f) + "\n\n" +
        "value: " + gf::niceNum(nextContracts[i].bubbleValueTarget, 1.0f)
      );
      m_choicesBubble[i] = &(m_animations[static_cast<uint8_t>(nextContracts[i].type)].get());
      buttons[i]->setString(city.name);
    }
  }

  void ContractSelectionEntity::pointTo(const gf::Vector2f& cursor)
  {
    m_widgets.pointTo(cursor);
  }

  void ContractSelectionEntity::triggerAction()
  {
    m_widgets.triggerAction();
  }

  void ContractSelectionEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
  {
    gf::Coordinates coords(target);

    constexpr float characterSize = 0.055f;
    const unsigned relativeCharacterSize = coords.getRelativeCharacterSize(characterSize);

    const float paragraphWidth = coords.getRelativeSize({0.25f, 0.0f}).width;
    const float paddingSize = coords.getRelativeSize({0.01f, 0.f}).x;

    gf::Text selectionTitle("Choose your next halt", m_font, relativeCharacterSize * 0.90);
    selectionTitle.setAnchor(gf::Anchor::BottomCenter);
    selectionTitle.setColor(gf::Color::White);
    selectionTitle.setPosition(coords.getRelativePoint({0.50f, 0.30f}));
    target.draw(selectionTitle, states);

    m_text1.setCharacterSize(relativeCharacterSize);
    m_text1.setParagraphWidth(paragraphWidth);
    m_text1.setAnchor(gf::Anchor::TopCenter);
    m_text1.setColor(gf::Color::White);
    m_text1.setPosition(coords.getRelativePoint({0.20f, 0.40f}));
    target.draw(m_text1,states);

    m_text2.setCharacterSize(relativeCharacterSize);
    m_text2.setParagraphWidth(paragraphWidth);
    m_text2.setAnchor(gf::Anchor::TopCenter);
    m_text2.setColor(gf::Color::White);
    m_text2.setPosition(coords.getRelativePoint({0.50f, 0.40f}));
    target.draw(m_text2,states);

    m_text3.setCharacterSize(relativeCharacterSize);
    m_text3.setParagraphWidth(paragraphWidth);
    m_text3.setAnchor(gf::Anchor::TopCenter);
    m_text3.setColor(gf::Color::White);
    m_text3.setPosition(coords.getRelativePoint({0.80f, 0.40f}));
    target.draw(m_text3,states);

    for (int i=0; i<3; i++) {
      gf::AnimatedSprite contractBubble;
      contractBubble.setAnimation(*(m_choicesBubble[i]));
      contractBubble.setAnchor(gf::Anchor::CenterLeft);
      contractBubble.setPosition(coords.getRelativePoint({0.21f + 0.3f*i, 0.65f}) - gf::Vector2f{60.0f, 5.0f});
      contractBubble.scale(0.4f);
      target.draw(contractBubble, states);
    }

    m_choice1.setCharacterSize(relativeCharacterSize);
    m_choice1.setParagraphWidth(paragraphWidth);
    m_choice1.setPadding(paddingSize);
    m_choice1.setAnchor(gf::Anchor::TopCenter);
    m_choice1.setPosition(coords.getRelativePoint({0.20f, 0.75f}));

    m_choice2.setCharacterSize(relativeCharacterSize);
    m_choice2.setParagraphWidth(paragraphWidth);
    m_choice2.setPadding(paddingSize);
    m_choice2.setAnchor(gf::Anchor::TopCenter);
    m_choice2.setPosition(coords.getRelativePoint({0.50f, 0.75f}));

    m_choice3.setCharacterSize(relativeCharacterSize);
    m_choice3.setParagraphWidth(paragraphWidth);
    m_choice3.setPadding(paddingSize);
    m_choice3.setAnchor(gf::Anchor::TopCenter);
    m_choice3.setPosition(coords.getRelativePoint({0.80f, 0.75f}));

    m_widgets.render(target, states);
  }

}
