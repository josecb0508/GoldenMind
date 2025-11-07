#include "Menu.hpp"
#include <iostream>

Menu::Menu(sf::RenderWindow &w) : window(w) 
{
    if (!font.loadFromFile("assets/fonts/BoldPixels.otf")) 
    {
        std::cerr << "No se pudo cargar la fuente assets/fonts/BoldPixels.otf\n";
    }
}
