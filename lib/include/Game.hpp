#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "MainMenu.hpp"
#include "MiniGameSelector.hpp"
#include "MentalMath.hpp"
#include "GameState.hpp"
#include "RepetirPatrones.hpp"
#include <string>

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    sf::RenderWindow window;
    GameState state;
    GameState lastState;

    MainMenu* mainMenu;
    MiniGameSelector* selector;
    MentalMath* mentalMath;
    RepetirPatrones* patternGame;

    sf::Music bgMusic;

    void playMusic(const std::string &filename);
    void processEvents();
};
