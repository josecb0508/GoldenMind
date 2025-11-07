#include "RepetirPatrones.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

static int randRange(int a, int b) { return a + (std::rand() % (b - a + 1)); }

RepetirPatrones::RepetirPatrones(sf::RenderWindow &w, GameState &s)
    : window(w), state(s), score(0), lives(3), gameOver(false),
      phase(WAITING), showIndex(0), waitingSoundEnd(false), canStart(false)
{
    std::srand((unsigned)std::time(nullptr));
    setupUI();
    setupButtons();
    setupLives();
    setupSounds();

    txtMessage.setString("Presiona INICIAR para comenzar");
    addToPattern();

    // Botón iniciar
    startButton.setSize({200.f, 50.f});
    startButton.setFillColor(sf::Color(100,200,100));
    startButton.setPosition(window.getSize().x/2.f - 100.f,
                            window.getSize().y/2.f + 200.f);

    startButtonText.setFont(font);
    startButtonText.setString("INICIAR");
    startButtonText.setCharacterSize(24);
    startButtonText.setFillColor(sf::Color::Black);
    sf::FloatRect tb = startButtonText.getLocalBounds();
    startButtonText.setOrigin(tb.left + tb.width/2.f, tb.top + tb.height/2.f);
    startButtonText.setPosition(startButton.getPosition().x + startButton.getSize().x/2.f,
                                startButton.getPosition().y + startButton.getSize().y/2.f);
}

RepetirPatrones::~RepetirPatrones() {}

void RepetirPatrones::setupUI() {
    if (!font.loadFromFile("assets/fonts/BoldPixels.otf"))
        std::cerr << "No se pudo cargar la fuente BoldPixels.otf\n";

    txtScore.setFont(font); txtScore.setCharacterSize(22); txtScore.setFillColor(sf::Color::Black);
    txtScore.setPosition(16.f, 10.f);

    txtBack.setFont(font); txtBack.setCharacterSize(18); txtBack.setFillColor(sf::Color(120,0,0));
    txtBack.setString("ESC para volver");
    txtBack.setPosition(16.f, window.getSize().y - 36.f);

    txtMessage.setFont(font); txtMessage.setCharacterSize(36); txtMessage.setFillColor(sf::Color::Red);
}

void RepetirPatrones::setupButtons() {
    buttons.clear();
    float size = 160.f;
    float gap = 18.f;
    float startX = window.getSize().x/2.f - size - gap/2.f;
    float startY = window.getSize().y/2.f - size - gap/2.f;

    sf::Color col[4] = { sf::Color(255,200,50), sf::Color(220,80,80),
                          sf::Color(90,200,90), sf::Color(80,140,240) };

    for (int i=0;i<4;i++){
        Buttons b;
        b.rect.setSize({size, size});
        b.rect.setPosition(startX + (i%2)*(size+gap), startY + (i/2)*(size+gap));
        b.baseColor = col[i];
        b.highlightColor = sf::Color(
            std::min(col[i].r+80,255),
            std::min(col[i].g+80,255),
            std::min(col[i].b+80,255)
        );
        b.rect.setFillColor(b.baseColor);
        b.id = i;
        buttons.push_back(b);
    }
}

void RepetirPatrones::setupLives() {
    if(!heartTexture.loadFromFile("assets/images/heart.png")) std::cerr<<"No se pudo cargar heart.png\n";
    heartSprite.setTexture(heartTexture);
    heartSprite.setScale(0.3f,0.3f);
}

void RepetirPatrones::setupSounds() {
    if (!birdBuf.loadFromFile("assets/audios/bird.wav")) std::cerr<<"No bird.wav\n";
    if (!dogBuf.loadFromFile("assets/audios/dog.wav")) std::cerr<<"No dog.wav\n";
    if (!cowBuf.loadFromFile("assets/audios/cow.wav")) std::cerr<<"No cow.wav\n";
    if (!catBuf.loadFromFile("assets/audios/cat.wav")) std::cerr<<"No cat.wav\n";
    if (!failBuf.loadFromFile("assets/audios/fail.wav")) std::cerr<<"No fail.wav\n";
    if (!correctBuf.loadFromFile("assets/audios/correct.wav")) std::cerr<<"No correct.wav\n";

    birdS.setBuffer(birdBuf); dogS.setBuffer(dogBuf);
    cowS.setBuffer(cowBuf); catS.setBuffer(catBuf);
    failS.setBuffer(failBuf); correctS.setBuffer(correctBuf);

    birdS.setVolume(60.f); dogS.setVolume(60.f);
    cowS.setVolume(60.f); catS.setVolume(60.f);
    failS.setVolume(60.f); correctS.setVolume(60.f);
}

void RepetirPatrones::addToPattern() {
    int next = randRange(0,3);
    pattern.push_back(next);
}

void RepetirPatrones::startRound() {
    phase = WAIT_BEFORE_SHOW;
    showIndex = 0;
    phaseClock.restart();
    playerInput.clear();
    waitingSoundEnd = false;
    txtMessage.setString("Observa la secuencia");
}

void RepetirPatrones::playButtonSound(int id) {
    switch(id) {
        case 0: birdS.play(); break;
        case 1: dogS.play(); break;
        case 2: cowS.play(); break;
        case 3: catS.play(); break;
    }
}

void RepetirPatrones::flashStep() 
{
    // Mostrar secuencia
    if (phase == WAIT_BEFORE_SHOW) {
        if (phaseClock.getElapsedTime().asSeconds() >= delayBetweenFlashes) {
            phase = SHOWING;
            phaseClock.restart();
            showIndex = 0;
            waitingSoundEnd = false;
        }
        return;
    }

    if (phase != SHOWING) return;

    if (showIndex >= pattern.size()) {
        phase = PLAYER_TURN;
        txtMessage.setString("Repite la secuencia");
        return;
    }

    float t = phaseClock.getElapsedTime().asSeconds();
    int id = buttons[pattern[showIndex]].id;

    if (!waitingSoundEnd && t >= delayBetweenFlashes) {
        buttons[id].rect.setFillColor(buttons[id].highlightColor);
        playButtonSound(id);
        waitingSoundEnd = true;
        phaseClock.restart();
    } else if (waitingSoundEnd && t >= flashOnTime) {
        buttons[id].rect.setFillColor(buttons[id].baseColor);
        showIndex++;
        waitingSoundEnd = false;
        phaseClock.restart();
    }
}

void RepetirPatrones::handlePlayerClick(int id) {
    if (!canStart || phase != PLAYER_TURN || gameOver) return;

    // Flash jugador
    buttons[id].rect.setFillColor(buttons[id].highlightColor);
    playerFlashing = true;
    playerFlashingID = id;
    playerFlashClock.restart();

    playButtonSound(id);
    playerInput.push_back(id);

    for(size_t i=0;i<playerInput.size();i++) {
        if(playerInput[i] != pattern[i]) {
            lives--;
            failS.play();
            playerInput.clear();
            if (lives <= 0) {
                gameOver = true;
                txtMessage.setString("¡Perdiste! Presiona Enter para reiniciar");
            } else {
                startRound();
            }
            return;
        }
    }

    if(playerInput.size() == pattern.size()) {
        score++;
        correctS.play();
        addToPattern();
        startRound();
    }
}

void RepetirPatrones::resetGame() {
    pattern.clear();
    playerInput.clear();
    score = 0;
    lives = 3;
    gameOver = false;
    addToPattern();
    txtMessage.setString("Presiona INICIAR para comenzar");
    canStart = false;
    phase = WAITING;
}

void RepetirPatrones::handleEvent(const sf::Event &event) {
    if (gameOver) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            resetGame();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            state = GameState::MINI_GAME_SELECTOR;
            resetGame();
        }
        return;
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        state = GameState::MINI_GAME_SELECTOR;
        resetGame();
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouseF((float)event.mouseButton.x, (float)event.mouseButton.y);

        if(!canStart && startButton.getGlobalBounds().contains(mouseF)) {
            canStart = true;
            startRound();
            return;
        }

        if(phase != PLAYER_TURN) return;

        for (auto &b : buttons) {
            if (b.rect.getGlobalBounds().contains(mouseF)) {
                handlePlayerClick(b.id);
                break;
            }
        }
    }
}

void RepetirPatrones::update() {
    txtScore.setString("Puntos: " + std::to_string(score));

    // Flash secuencia
    if(phase == SHOWING || phase == WAIT_BEFORE_SHOW)
        flashStep();

    // Flash jugador
    if (playerFlashing) {
        if (playerFlashClock.getElapsedTime().asSeconds() >= playerFlashTime) {
            buttons[playerFlashingID].rect.setFillColor(buttons[playerFlashingID].baseColor);
            playerFlashing = false;
        }
    }
}

void RepetirPatrones::render() {
    window.clear(sf::Color::White);

    for(auto &b : buttons) window.draw(b.rect);

    for(int i=0;i<lives;i++){
        heartSprite.setPosition(window.getSize().x-50.f-i*40.f,10.f);
        window.draw(heartSprite);
    }

    window.draw(txtScore);
    window.draw(txtBack);

    if(!canStart) {
        window.draw(startButton);
        window.draw(startButtonText);
    }

    if(!txtMessage.getString().isEmpty()){
        sf::FloatRect bounds = txtMessage.getLocalBounds();
        txtMessage.setOrigin(bounds.left + bounds.width/2.f, bounds.top + bounds.height/2.f);
        txtMessage.setPosition(window.getSize().x/2.f, 60.f);
        window.draw(txtMessage);
    }

    window.display();
}
