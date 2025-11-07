#include "Game.hpp"
#include <iostream>

Game::Game()
    : window(sf::VideoMode(800, 600), "GoldenMind"),
      state(GameState::MENU), lastState(GameState::MENU)
{
    window.setFramerateLimit(60);

    mainMenu = new MainMenu(window, state);
    selector = new MiniGameSelector(window, state);
    mentalMath = new MentalMath(window, state);
    patternGame = new RepetirPatrones(window, state); 

    playMusic("assets/audios/mainmenu.wav");
}

Game::~Game() {
    delete mainMenu;
    delete selector;
    delete mentalMath;
    delete patternGame;
}

void Game::run()
{
    GameState lastState = state;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (state == GameState::MENU) mainMenu->handleEvent(event);
            else if (state == GameState::MINI_GAME_SELECTOR) selector->handleEvent(event);
            else if (state == GameState::MENTAL_MATH) mentalMath->handleEvent(event);
            else if (state == GameState::PATTERN_GAME) patternGame->handleEvent(event);
        }

        if (state != lastState) {
            lastState = state;
            switch (state) {
                case GameState::MENU: playMusic("assets/audios/mainmenu.wav"); break;
                case GameState::MINI_GAME_SELECTOR: playMusic("assets/audios/selectormenu.wav"); break;
                case GameState::MENTAL_MATH: playMusic("assets/audios/minigame.wav"); break;
                case GameState::PATTERN_GAME: playMusic("assets/audios/patternmenu.wav"); break;
            }
        }

        if (state == GameState::MENU) mainMenu->update();
        else if (state == GameState::MINI_GAME_SELECTOR) selector->update();
        else if (state == GameState::MENTAL_MATH) mentalMath->update();
        else if (state == GameState::PATTERN_GAME) patternGame->update();

        window.clear(sf::Color::White);
        if (state == GameState::MENU) mainMenu->render();
        else if (state == GameState::MINI_GAME_SELECTOR) selector->render();
        else if (state == GameState::MENTAL_MATH) mentalMath->render();
        else if (state == GameState::PATTERN_GAME) patternGame->render();
        window.display();
    }
}
void Game::playMusic(const std::string &filename)
{
    if (bgMusic.getStatus() == sf::Music::Playing)
        bgMusic.stop();

    if (!bgMusic.openFromFile(filename)) {
        std::cerr << "Warning: failed to open music: " << filename << std::endl;
        return;
    }

    bgMusic.setLoop(true);
    bgMusic.play();
}
