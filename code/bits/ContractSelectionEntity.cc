#include "ContractSelectionEntity.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/Text.h>

#include "GameHub.h"

namespace be {

  ContractSelectionEntity::ContractSelectionEntity(GameHub& game)
  : m_game(game)
  , m_font(game.resources.getFont("DejaVuSans.ttf"))
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
      button.setAnchor(gf::Anchor::TopLeft);
      button.setAlignment(gf::Alignment::Center);
      button.setCallback(callback);
      m_widgets.addWidget(button);
    };

    setupButton(m_choice1, [&] () {
      gf::Log::debug("Choice 1\n");
      if (m_game.state.contract.originCity <= 0) {
        m_game.state.contract.targetCity = 1;
      } else {
        m_game.state.contract.targetCity = 0;
      }

      assert(m_game.state.contract.targetCity >= 0 && m_game.state.contract.targetCity < CityCount);
      assert(m_choice1.getString() == m_game.state.cities[m_game.state.contract.targetCity].name);

      gf::Log::debug("New target City: %s\n", m_game.state.cities[m_game.state.contract.targetCity].name.c_str());

      m_game.replaceAllScenes(m_game.world);
    });

    setupButton(m_choice2, [&] () {
      gf::Log::debug("Choice 2\n");
      if (m_game.state.contract.originCity <= 1) {
        m_game.state.contract.targetCity = 2;
      } else {
        m_game.state.contract.targetCity = 1;
      }


      assert(m_game.state.contract.targetCity >= 0 && m_game.state.contract.targetCity < CityCount);
      assert(m_choice2.getString() == m_game.state.cities[m_game.state.contract.targetCity].name);

      gf::Log::debug("New target City: %s\n", m_game.state.cities[m_game.state.contract.targetCity].name.c_str());

      m_game.replaceAllScenes(m_game.world);
    });

    setupButton(m_choice3, [&] () {
      gf::Log::debug("Choice 3\n");
      if (m_game.state.contract.originCity <= 2) {
        m_game.state.contract.targetCity = 3;
      } else {
        m_game.state.contract.targetCity = 2;
      }


      assert(m_game.state.contract.targetCity >= 0 && m_game.state.contract.targetCity < CityCount);
      assert(m_choice3.getString() == m_game.state.cities[m_game.state.contract.targetCity].name);

      gf::Log::debug("New target City: %s\n", m_game.state.cities[m_game.state.contract.targetCity].name.c_str());

      m_game.replaceAllScenes(m_game.world);
    });
  }

  void ContractSelectionEntity::updateContracts()
  {
    const std::array<gf::TextButtonWidget*, 3> buttons = {
      &m_choice1,
      &m_choice2,
      &m_choice3,
    };

    for (int i = 0; i < m_game.state.contract.originCity; ++i) {
      buttons[i]->setString(m_game.state.cities[i].name);
    }

    for (int i = m_game.state.contract.originCity + 1; i < m_game.state.cities.size(); ++i) {
      buttons[i - 1]->setString(m_game.state.cities[i].name);
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

    constexpr gf::Vector2f ButtonStartPoint = gf::vec(0.60f, 0.30f);
    constexpr float spaceBetweenButton = 0.06f;
    constexpr gf::Vector2f backgroundSize(0.35f, 0.40f);

    gf::Text currentCity("You are at " + m_game.state.cities[m_game.state.contract.originCity].name, m_font, relativeCharacterSize * 0.90);
    currentCity.setAnchor(gf::Anchor::BottomLeft);
    currentCity.setColor(gf::Color::White);
    currentCity.setPosition(coords.getRelativePoint(gf::vec(0.05f, ButtonStartPoint.y - spaceBetweenButton)));
    target.draw(currentCity, states);

    const float paragraphWidth = coords.getRelativeSize(backgroundSize - 0.05f).x;
    const float paddingSize = coords.getRelativeSize({0.01f, 0.f}).x;

    gf::Text selectionTitle("Choose your next halt", m_font, relativeCharacterSize * 0.90);
    selectionTitle.setAnchor(gf::Anchor::BottomLeft);
    selectionTitle.setColor(gf::Color::White);
    selectionTitle.setPosition(coords.getRelativePoint(ButtonStartPoint + gf::vec(0.0f, - spaceBetweenButton)));
    target.draw(selectionTitle, states);

    m_choice1.setCharacterSize(relativeCharacterSize);
    m_choice1.setPosition(coords.getRelativePoint(ButtonStartPoint));
    m_choice1.setParagraphWidth(paragraphWidth);
    m_choice1.setPadding(paddingSize);

    m_choice2.setCharacterSize(relativeCharacterSize);
    m_choice2.setPosition(coords.getRelativePoint(ButtonStartPoint + gf::vec(0.0f, (characterSize + spaceBetweenButton) * 1.0f)));
    m_choice2.setParagraphWidth(paragraphWidth);
    m_choice2.setPadding(paddingSize);

    m_choice3.setCharacterSize(relativeCharacterSize);
    m_choice3.setPosition(coords.getRelativePoint(ButtonStartPoint + gf::vec(0.0f, (characterSize + spaceBetweenButton) * 2.0f)));
    m_choice3.setParagraphWidth(paragraphWidth);
    m_choice3.setPadding(paddingSize);

    m_widgets.render(target, states);
  }

}
