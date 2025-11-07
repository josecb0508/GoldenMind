#include "MainMenu.hpp"
#include "MiniGameSelector.hpp"
#include "GameState.hpp"

MainMenu::MainMenu(sf::RenderWindow &w, GameState &s)
    : Menu(w), state(s)
{
    sf::Color gold(212, 175, 55);

    title.setFont(font);
    title.setString("GoldenMind");
    title.setCharacterSize(60);
    title.setFillColor(gold);

    setupButton(playButton, playText, window.getSize().y * 0.4f);
    playText.setString("Jugar");

    setupButton(exitButton, exitText, window.getSize().y * 0.55f);
    exitText.setString("Salir");

    updateButtonPositions();
}

void MainMenu::setupButton(sf::RectangleShape& button, sf::Text& text, float y)
{
    button.setSize(sf::Vector2f(200, 60));
    button.setFillColor(sf::Color(50, 50, 50));

    text.setFont(font);
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::White);

    float centerX = window.getSize().x / 2.f;
    button.setPosition(centerX - button.getSize().x / 2.f, y);

    updateButtonTextPosition(button, text);
}

void MainMenu::updateButtonTextPosition(sf::RectangleShape& button, sf::Text& text)
{
    sf::FloatRect tBounds = text.getLocalBounds();
    text.setOrigin(tBounds.left + tBounds.width / 2.f, tBounds.top + tBounds.height / 2.f);
    text.setPosition(button.getPosition().x + button.getSize().x / 2.f, button.getPosition().y + button.getSize().y / 2.f);
}

void MainMenu::updateButtonPositions()
{
    float centerX = window.getSize().x / 2.f;
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    title.setPosition(centerX, window.getSize().y * 0.2f);
    playButton.setPosition(centerX - playButton.getSize().x / 2.f, window.getSize().y * 0.4f);
    exitButton.setPosition(centerX - exitButton.getSize().x / 2.f, window.getSize().y * 0.55f);
    updateButtonTextPosition(playButton, playText);
    updateButtonTextPosition(exitButton, exitText);
}

void MainMenu::update()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    playButton.setFillColor(playButton.getGlobalBounds().contains(mousePos.x, mousePos.y) ? sf::Color(150, 150, 50) : sf::Color(50, 50, 50));

    exitButton.setFillColor(exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y) ? sf::Color(150, 150, 50) : sf::Color(50, 50, 50));
}

void MainMenu::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);

        if (playButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            state = GameState::MINI_GAME_SELECTOR;
        }
        else if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            window.close();
        }
    }
    else if (event.type == sf::Event::Resized)
    {
        updateButtonPositions();
    }
}

void MainMenu::render()
{
    window.draw(title);
    window.draw(playButton);
    window.draw(playText);
    window.draw(exitButton);
    window.draw(exitText);
}
