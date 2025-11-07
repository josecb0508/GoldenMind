#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "GameState.hpp"
#include <vector>
#include <string>

class MiniGameSelector : public Menu
{
private:
    GameState &state;
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> labels;

    void updateButtonPositions();

public:
    MiniGameSelector(sf::RenderWindow &w, GameState &state);
    void update() override;
    void render() override;
    void handleEvent(const sf::Event &event);
};
