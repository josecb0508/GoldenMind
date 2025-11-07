#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameState.hpp"
#include <string>
#include <vector>

struct ExprNode
{
    std::string value;
    ExprNode* left;
    ExprNode* right;
    ExprNode(const std::string &v) : value(v), left(nullptr), right(nullptr) {}
};

struct Button
{
    sf::RectangleShape box;
    sf::Text label;
    sf::Color normalColor;
    sf::Color hoverColor;
};

class MentalMath
{
private:
    sf::RenderWindow &window;
    GameState &state;

    sf::Font font;
    sf::Text txtOperation;
    sf::Text txtAnswer;
    sf::Text txtScore;
    sf::Text txtBack;
    sf::Text txtTimer;

    sf::RectangleShape answerBox;
    sf::RectangleShape timerBar;
    float timerBarMaxWidth;

    ExprNode* root;
    int score;
    int lives;
    std::string playerInput;
    bool gameOver;

    enum class Difficulty { NONE, EASY, MEDIUM, HARD };
    Difficulty difficulty;
    std::vector<Button> difficultyButtons;

    float timeLimit;  // tiempo base
    sf::Clock clock;

    sf::Texture heartTexture;
    sf::Sprite heartSprite;

    sf::SoundBuffer correctBuffer;
    sf::SoundBuffer failBuffer;
    sf::SoundBuffer failedBuffer;
    sf::Sound correctSound;
    sf::Sound failSound;
    sf::Sound failedSound;

    // Funciones privadas
    void generateOperation();
    void generateExpressionTree();
    void deleteTree(ExprNode* node);
    int evaluate(ExprNode* node) const;
    std::string buildInfixString(ExprNode* node) const;
    std::vector<std::string> infixToPostfix(const std::vector<std::string>& infix);
    ExprNode* buildTreeFromPostfix(const std::vector<std::string>& postfix);
    bool isOperator(const std::string& s) const;
    void drawLives();
    void resetGame();
    void setupDifficultyButtons();
    void handleDifficultySelection(const sf::Event &event);
    void updateTimer();
    void checkTimeExpired();

public:
    MentalMath(sf::RenderWindow &w, GameState &s);
    ~MentalMath();

    void update();
    void render();
    void handleEvent(const sf::Event &event);
};
