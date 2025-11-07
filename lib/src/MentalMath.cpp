#include "MentalMath.hpp"
#include <cstdlib>
#include <ctime>
#include <stack>
#include <iostream>

static int randRange(int a, int b) { return a + (std::rand() % (b - a + 1)); }

MentalMath::MentalMath(sf::RenderWindow &w, GameState &s)
    : window(w), state(s), root(nullptr), score(0), lives(3), playerInput(""),
      gameOver(false), difficulty(Difficulty::NONE), timeLimit(15.f)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    if (!font.loadFromFile("assets/fonts/BoldPixels.otf"))
        std::cerr << "No se pudo cargar fuente\n";

    txtOperation.setFont(font); txtOperation.setCharacterSize(54); txtOperation.setFillColor(sf::Color::Black);
    txtAnswer.setFont(font); txtAnswer.setCharacterSize(36); txtAnswer.setFillColor(sf::Color::Black);
    txtScore.setFont(font); txtScore.setCharacterSize(24); txtScore.setFillColor(sf::Color::Black);
    txtScore.setPosition(16.f, 12.f); txtScore.setString("Puntos: 0");
    txtBack.setFont(font); txtBack.setCharacterSize(18); txtBack.setFillColor(sf::Color(120,0,0));
    txtBack.setString("ESC para volver"); txtBack.setPosition(16.f, window.getSize().y - 40.f);

    answerBox.setSize({360.f,80.f});
    answerBox.setFillColor(sf::Color(240,240,240));
    answerBox.setOutlineColor(sf::Color::Black); answerBox.setOutlineThickness(3.f);

    if(!heartTexture.loadFromFile("assets/images/heart.png")) std::cerr<<"No se pudo cargar heart.png\n";
    heartSprite.setTexture(heartTexture); heartSprite.setScale(0.3f,0.3f);

    if(!correctBuffer.loadFromFile("assets/audios/correct.wav")) std::cerr<<"No se pudo cargar correct.wav\n";
    if(!failBuffer.loadFromFile("assets/audios/fail.wav")) std::cerr<<"No se pudo cargar fail.wav\n";
    if(!failedBuffer.loadFromFile("assets/audios/failed.wav")) std::cerr<<"No se pudo cargar failed.wav\n";

    correctSound.setBuffer(correctBuffer); correctSound.setVolume(50.f);
    failSound.setBuffer(failBuffer); failSound.setVolume(50.f);
    failedSound.setBuffer(failedBuffer); failedSound.setVolume(50.f);

    timerBarMaxWidth = 300.f;
    timerBar.setSize({timerBarMaxWidth, 20.f});
    timerBar.setFillColor(sf::Color::Green);
    timerBar.setPosition(window.getSize().x - timerBarMaxWidth - 16.f, 50.f);

    setupDifficultyButtons();
}

MentalMath::~MentalMath() { deleteTree(root); }

void MentalMath::setupDifficultyButtons()
{
    std::vector<std::string> labels = {"Nivel 1", "Nivel 2", "Nivel 3"};
    float yStart = 200.f;
    for(int i=0;i<3;i++)
    {
        Button btn;
        btn.box.setSize({300.f,80.f});
        btn.box.setFillColor(sf::Color(180,180,180));
        btn.box.setPosition(window.getSize().x/2.f - 150.f, yStart + i*120.f);
        btn.normalColor = sf::Color(180,180,180);
        btn.hoverColor = sf::Color(220,220,0);

        btn.label.setFont(font);
        btn.label.setString(labels[i]);
        btn.label.setCharacterSize(36);
        btn.label.setFillColor(sf::Color::Black);
        sf::FloatRect bounds = btn.label.getLocalBounds();
        btn.label.setOrigin(bounds.left + bounds.width/2.f, bounds.top + bounds.height/2.f);
        btn.label.setPosition(btn.box.getPosition().x + btn.box.getSize().x/2.f, btn.box.getPosition().y + btn.box.getSize().y/2.f);
        difficultyButtons.push_back(btn);
    }
}

void MentalMath::handleDifficultySelection(const sf::Event &event)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for(int i=0;i<difficultyButtons.size();i++)
    {
        Button &btn = difficultyButtons[i];
        btn.box.setFillColor(btn.box.getGlobalBounds().contains(mousePos) ? btn.hoverColor : btn.normalColor);

        if(event.type==sf::Event::MouseButtonPressed && event.mouseButton.button==sf::Mouse::Left)
        {
            if(btn.box.getGlobalBounds().contains(mousePos))
            {
                difficulty = static_cast<Difficulty>(i+1);

                score = 0;
                lives = 3;
                playerInput = "";
                gameOver = false;
                timerBar.setSize({timerBarMaxWidth, timerBar.getSize().y});
                timerBar.setFillColor(sf::Color::Green);

                generateOperation();
                clock.restart();  
            }
        }
    }
}

void MentalMath::updateTimer()
{
    if(gameOver || difficulty==Difficulty::NONE)
    { 
        return;
    }
    float dynamicLimit = timeLimit;
    if(score>=5) 
    {
        dynamicLimit = 10.f;
    }
    if(score>=10)
    { 
        dynamicLimit = 5.f;
    }
    if(dynamicLimit<1.f)
    {
        dynamicLimit=1.f;
    }

    float elapsed = clock.getElapsedTime().asSeconds();
    float remaining = dynamicLimit - elapsed;
    if(remaining<0)
    {
        remaining=0;
    }

    float width = timerBarMaxWidth * (remaining / dynamicLimit);
    timerBar.setSize({width, timerBar.getSize().y});
    timerBar.setFillColor(remaining <= dynamicLimit*0.3f ? sf::Color::Red : sf::Color::Green);

    checkTimeExpired();
}

void MentalMath::checkTimeExpired()
{
    if(gameOver || difficulty==Difficulty::NONE) return;

    float dynamicLimit = timeLimit;
    if(score>=5) 
    {
        dynamicLimit = 10.f;
    }
    if(score>=10)
    { 
        dynamicLimit = 5.f;
    }
    if(dynamicLimit<1.f)
    {
        dynamicLimit=1.f;
    }

    if(clock.getElapsedTime().asSeconds()>=dynamicLimit)
    {
        lives--;
        failSound.play();
        if(lives<=0){ gameOver=true; failedSound.play(); }
        else generateOperation();
        clock.restart();
    }
}

void MentalMath::deleteTree(ExprNode* n)
{ 
    if(!n)
    {
        return; 
    }
    deleteTree(n->left); 
    deleteTree(n->right); 
    delete n; 
}
bool MentalMath::isOperator(const std::string &s) const 
{
     return s=="+"||s=="-"||s=="*"||s=="/"; 
}

std::vector<std::string> MentalMath::infixToPostfix(const std::vector<std::string> &infix)
{
    std::vector<std::string> out; std::stack<std::string> ops;
    auto prec = [](const std::string &op)
    { 
        if(op=="+"||op=="-") return 1; if(op=="*"||op=="/") return 2; return 0;};
    for(auto &tok: infix)
    { 
        if(isOperator(tok))
        { while(!ops.empty() && isOperator(ops.top()) && prec(ops.top())>=prec(tok))
            { 
                out.push_back(ops.top()); ops.pop(); 
            } 
            ops.push(tok);
        } 
        else
        { 
            out.push_back(tok);
        }
    }
    while(!ops.empty()){ out.push_back(ops.top()); ops.pop(); }
    return out;
}

ExprNode* MentalMath::buildTreeFromPostfix(const std::vector<std::string> &postfix)
{
    std::stack<ExprNode*> st;
    for(auto &tok: postfix)
    {
        if(!isOperator(tok)) 
        {
            st.push(new ExprNode(tok));
        }
        else
        {
            if(st.size()<2)
            {
                while(!st.empty())
                { 
                    delete st.top(); st.pop(); 
                } 
                return nullptr; 
            }
            ExprNode* r=st.top(); st.pop(); ExprNode* l=st.top(); st.pop();
            ExprNode* p=new ExprNode(tok); p->left=l; p->right=r; st.push(p);
        }
    }
    if(st.empty())
    {
        return nullptr;
    }
    ExprNode* rootLocal=st.top(); 
    st.pop(); 
    return rootLocal;
}

void MentalMath::generateExpressionTree()
{
    deleteTree(root); root=nullptr;

    int numOperands=2;
    std::vector<int> nums;
    std::vector<std::string> opsSet;

    if(difficulty==Difficulty::EASY) 
    { 
        opsSet = {"+","-"}; 

        int a = randRange(1,10);
        int b = randRange(1,10);

        std::string op = opsSet[randRange(0, opsSet.size()-1)];

        if(op == "-") 
        {
            if(a < b) std::swap(a,b);
        }

        nums.push_back(a);
        nums.push_back(b);

        std::vector<std::string> infix;
        infix.push_back(std::to_string(nums[0]));
        infix.push_back(op);
        infix.push_back(std::to_string(nums[1]));

        std::vector<std::string> postfix = infixToPostfix(infix);
        root = buildTreeFromPostfix(postfix);
        return;
    }
    else if(difficulty==Difficulty::MEDIUM)
    {
        for(int i=0;i<numOperands;i++) nums.push_back(randRange(1,20));
        opsSet={"+","-","*"};
    }
    else 
    {
        for(int i=0;i<numOperands;i++) nums.push_back(randRange(1,50));
        opsSet={"+","-","*","/"};
    }

    std::vector<std::string> infix;
    infix.push_back(std::to_string(nums[0]));
    for(int i=0;i<numOperands-1;i++)
    {
        std::string op = opsSet[randRange(0,opsSet.size()-1)];
        if(op=="/")
        { 
            int right=nums[i+1]; 
            if(right==0)
            {
                right=1;
            } 
            nums[i]=right*randRange(1,5); 
            infix.back()=std::to_string(nums[i]);
        }
        infix.push_back(op); 
        infix.push_back(std::to_string(nums[i+1]));
    }

    std::vector<std::string> postfix = infixToPostfix(infix);
    root = buildTreeFromPostfix(postfix);
}

int MentalMath::evaluate(ExprNode* node) const
{
    if(!node) return 0;
    if(!node->left && !node->right) return std::stoi(node->value);
    int L=evaluate(node->left); int R=evaluate(node->right);
    if(node->value=="+") return L+R;
    if(node->value=="-") return L-R;
    if(node->value=="*") return L*R;
    if(node->value=="/") return (R!=0)? L/R:0;
    return 0;
}

std::string MentalMath::buildInfixString(ExprNode* node) const
{
    if(!node) return "";
    if(!node->left && !node->right) return node->value;
    return "("+buildInfixString(node->left)+" "+node->value+" "+buildInfixString(node->right)+")";
}

void MentalMath::generateOperation()
{
    playerInput=""; txtAnswer.setString("");
    generateExpressionTree();
    std::string opStr = buildInfixString(root) + " = ?";
    txtOperation.setString(opStr);
    sf::FloatRect opBounds = txtOperation.getLocalBounds();
    txtOperation.setOrigin(opBounds.left + opBounds.width/2.f, opBounds.top+opBounds.height/2.f);
    txtOperation.setPosition(window.getSize().x/2.f, window.getSize().y*0.28f);

    answerBox.setPosition(window.getSize().x/2.f - answerBox.getSize().x/2.f, window.getSize().y*0.45f);
    txtAnswer.setPosition(answerBox.getPosition().x + 14.f, answerBox.getPosition().y + 10.f);

    txtScore.setString("Puntos: "+std::to_string(score));
    clock.restart();
}

void MentalMath::handleEvent(const sf::Event &event)
{
    if(difficulty==Difficulty::NONE){ handleDifficultySelection(event); return; }

    if(gameOver)
    {
        if(event.type==sf::Event::KeyPressed && event.key.code==sf::Keyboard::Enter)
        {
            score = 0; lives = 3; playerInput = ""; gameOver = false;
            generateOperation();
            clock.restart();
        }
        return;
    }

    if(event.type==sf::Event::KeyPressed)
    {
        if(event.key.code==sf::Keyboard::Escape)
        {
            score = 0; lives = 3; playerInput = ""; gameOver = false; difficulty = Difficulty::NONE;
            timerBar.setSize({timerBarMaxWidth, timerBar.getSize().y});
            timerBar.setFillColor(sf::Color::Green);

            state=GameState::MINI_GAME_SELECTOR;
            return;
        }
        else if(event.key.code==sf::Keyboard::Enter)
        {
            if(playerInput.empty())
            {
                return;
            }
            bool correct=false;
            try
            {
                correct = (std::stoi(playerInput)==evaluate(root));
            }
            catch(...)
            { 
                correct=false;
            }
            if(correct)
            {
                score++; correctSound.play(); 
            }
            else
            { 
                lives--; 
                failSound.play(); 
                if(lives<=0)
                { 
                    gameOver=true; 
                    failedSound.play(); 
                } 
            }

            txtScore.setString("Puntos: "+std::to_string(score));
            if(!gameOver)
            {
                generateOperation();
            }
        }
    }

    if(event.type==sf::Event::TextEntered)
    {
        char c=static_cast<char>(event.text.unicode);
        if(c==8)
        { 
            if(!playerInput.empty()) 
            {
                playerInput.pop_back(); 
            }
            txtAnswer.setString(playerInput); 
            return; 
        }
        if((c>='0' && c<='9') || (c=='-' && playerInput.empty())) 
        { 
            playerInput.push_back(c); 
            txtAnswer.setString(playerInput);
        }
    }
}

void MentalMath::update()
{
    if(difficulty!=Difficulty::NONE)
    {
        updateTimer();
    }
    txtAnswer.setString(playerInput);
}

void MentalMath::drawLives()
{
    for(int i=0;i<lives;i++)
    { 
        heartSprite.setPosition(window.getSize().x-50.f-i*40.f,10.f); 
        window.draw(heartSprite);
    }
}

void MentalMath::render()
{
    window.clear(sf::Color::White);

    if(difficulty==Difficulty::NONE)
    {
        for(auto &btn:difficultyButtons)
        { 
            window.draw(btn.box);
            window.draw(btn.label);
        }
    }
    else
    {
        window.draw(txtOperation); 
        window.draw(answerBox);
        window.draw(txtAnswer);
        window.draw(txtScore); 
        window.draw(txtBack); 
        drawLives();
        window.draw(timerBar); 

        if(gameOver)
        {
            sf::Text t; 
            t.setFont(font); 
            t.setCharacterSize(48); 
            t.setFillColor(sf::Color::Red);
            t.setString("Perdiste!\nEnter para reiniciar");
            sf::FloatRect b=t.getLocalBounds();
            t.setOrigin(b.left+b.width/2.f,b.top+b.height/2.f);
            t.setPosition(window.getSize().x/2.f, window.getSize().y/2.f);
            window.draw(t);
        }
    }

    window.display();
}
