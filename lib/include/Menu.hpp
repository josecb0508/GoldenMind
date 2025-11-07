#pragma once
#include <SFML/Graphics.hpp>

class Menu 
{
protected:
    sf::RenderWindow &window;
    sf::Font font;

public:
    Menu(sf::RenderWindow &w);
    virtual ~Menu() = default;
    virtual void update() = 0;
    virtual void render() = 0;
};
