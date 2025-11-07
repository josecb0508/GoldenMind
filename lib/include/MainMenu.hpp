#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "GameState.hpp"

class MainMenu : public Menu 
{
private:
    GameState &state;
    sf::Text title;
    sf::Text playText;
    sf::Text exitText;

    sf::RectangleShape playButton;
    sf::RectangleShape exitButton;

    void setupButton(sf::RectangleShape& button, sf::Text& text, float y);
    void updateButtonTextPosition(sf::RectangleShape& button, sf::Text& text);

public: 
    MainMenu(sf::RenderWindow &w, GameState &state);
    void update() override;
    void render() override;
    void handleEvent(const sf::Event &event);   
    void updateButtonPositions();
};
