#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "GameState.hpp"

enum Phase { WAITING, SHOWING, PLAYER_TURN, WAIT_BEFORE_SHOW };

struct Buttons
{
    sf::RectangleShape rect; 
    sf::Color baseColor;
    sf::Color highlightColor;
    int id;
};

class RepetirPatrones 
{
private:
    sf::RenderWindow &window;
    GameState &state;

    std::vector<Buttons> buttons;
    std::vector<int> pattern;
    std::vector<int> playerInput;

    int score;
    int lives;
    bool gameOver;

    Phase phase;
    size_t showIndex;
    sf::Clock phaseClock;
    bool waitingSoundEnd;

    float flashOnTime = 0.6f;         
    float delayBetweenFlashes = 0.8f;  

    sf::Font font;
    sf::Text txtScore, txtBack, txtMessage;
    sf::RectangleShape startButton;
    sf::Text startButtonText;
    bool canStart;

    sf::SoundBuffer birdBuf, dogBuf, cowBuf, catBuf, failBuf, correctBuf;
    sf::Sound birdS, dogS, cowS, catS, failS, correctS;

    sf::Texture heartTexture;
    sf::Sprite heartSprite;

    bool playerFlashing = false;
    int playerFlashingID = -1;
    sf::Clock playerFlashClock;
    float playerFlashTime = 0.3f;

    void setupUI();
    void setupButtons();
    void setupLives();
    void setupSounds();
    void addToPattern();
    void startRound();
    void playButtonSound(int id);
    void flashStep();
    void handlePlayerClick(int id);

public:
    RepetirPatrones(sf::RenderWindow &w, GameState &s);
    ~RepetirPatrones();

    void resetGame();
    void handleEvent(const sf::Event &event);
    void update();
    void render();
};
