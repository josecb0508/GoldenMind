#include "MiniGameSelector.hpp"

MiniGameSelector::MiniGameSelector(sf::RenderWindow &w, GameState &s)
    : Menu(w), state(s)
{
    std::vector<std::string> options = {
        "Calculo Mental",
        "Repetir Patrones",
        "Volver"
    };

    float startY = 180.f;
    for (auto &textStr : options) {
        sf::RectangleShape button(sf::Vector2f(360, 70));
        button.setFillColor(sf::Color(50, 50, 50));
        buttons.push_back(button);

        sf::Text text;
        text.setFont(font);
        text.setString(textStr);
        text.setCharacterSize(28);
        labels.push_back(text);

        startY += 90;
    }

    updateButtonPositions();
}

void MiniGameSelector::updateButtonPositions()
{
    float centerX = window.getSize().x / 2.f;

    for (size_t i = 0; i < buttons.size(); i++) {
        float y = 160 + i * 100;
        buttons[i].setPosition(centerX - buttons[i].getSize().x/2.f, y);

        sf::FloatRect bounds = labels[i].getLocalBounds();
        labels[i].setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        labels[i].setPosition(centerX, y + buttons[i].getSize().y/2.f - 6.f);
    }
}

void MiniGameSelector::update()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    for (size_t i = 0; i < buttons.size(); i++) {
        if (buttons[i].getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
            buttons[i].setFillColor(sf::Color(150, 150, 50));
        else
            buttons[i].setFillColor(sf::Color(50, 50, 50));
    }
}

void MiniGameSelector::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mouse(event.mouseButton.x, event.mouseButton.y);

        if (buttons.size() > 0 && buttons[0].getGlobalBounds().contains((float)mouse.x, (float)mouse.y)) {
            state = GameState::MENTAL_MATH;
        }
        if (buttons.size() > 1 && buttons[1].getGlobalBounds().contains((float)mouse.x, (float)mouse.y)) {
            state = GameState::PATTERN_GAME;
        }
        if (buttons.size() > 2 && buttons[2].getGlobalBounds().contains((float)mouse.x, (float)mouse.y)) {
            state = GameState::MENU;
        }
    }

    if (event.type == sf::Event::Resized)
        updateButtonPositions();
}

void MiniGameSelector::render()
{
    for (auto &b : buttons) window.draw(b);
    for (auto &t : labels) window.draw(t);
}
