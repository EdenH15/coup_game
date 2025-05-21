#include "GameGUI.h"
#include <iostream>
#include <cmath>
#include <SFML/System.hpp>
#include "CoupGame/Roles/Baron.h"
#include "CoupGame/Roles/General.h"
#include "CoupGame/Roles/Spy.h"
#include "CoupGame/Roles/Governor.h"
#include "CoupGame/Roles/Judge.h"

using namespace CoupG;

GameGUI::GameGUI(const int numPlayers)
    : window(sf::VideoMode(1000, 600), "Coup Game"), game(numPlayers) {
    font.loadFromFile("arial.ttf");

    const float windowWidth = 1000;
    const float windowHeight = 600;

    // Title
    title.setFont(font);
    title.setString("Welcome to Coup Game!");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    title.setPosition(windowWidth / 2, 100);

    // Start Button
    startButton.setSize(sf::Vector2f(200, 50));
    startButton.setFillColor(sf::Color(200, 100, 100));
    startButton.setPosition((windowWidth - startButton.getSize().x) / 2, 200);

    startText.setFont(font);
    startText.setString("START GAME");
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color::White);
    sf::FloatRect startTextBounds = startText.getLocalBounds();
    startText.setOrigin(startTextBounds.width / 2, startTextBounds.height / 2);
    startText.setPosition(windowWidth / 2, 210 + 10); // +10 ליישור אנכי בתוך הכפתור

    // Input Box
    inputBox.setSize(sf::Vector2f(300, 40));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setPosition((windowWidth - inputBox.getSize().x) / 2, 300);

    inputText.setFont(font);
    inputText.setCharacterSize(22);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition((windowWidth - inputBox.getSize().x) / 2 + 10, 305); // קצת פנימה מהקופסה

    // Add Player Button
    addPlayerButton.setSize(sf::Vector2f(150, 40));
    addPlayerButton.setFillColor(sf::Color(200, 100, 100));
    addPlayerButton.setPosition((windowWidth - addPlayerButton.getSize().x) / 2, 360);

    addPlayerText.setFont(font);
    addPlayerText.setString("Add Player");
    addPlayerText.setCharacterSize(20);
    addPlayerText.setFillColor(sf::Color::Black);
    sf::FloatRect addPlayerTextBounds = addPlayerText.getLocalBounds();
    addPlayerText.setOrigin(addPlayerTextBounds.width / 2, addPlayerTextBounds.height / 2);
    addPlayerText.setPosition(windowWidth / 2, 368 + 8); // יישור אנכי בתוך הכפתור

    // Next Button
    nextButton.setSize(sf::Vector2f(200, 50));
    nextButton.setFillColor(sf::Color(200, 100, 100));
    nextButton.setPosition((windowWidth - nextButton.getSize().x) / 2, 540);

    nextButtonText.setFont(font);
    nextButtonText.setString("Next To The Game");
    nextButtonText.setCharacterSize(22);
    nextButtonText.setFillColor(sf::Color::White);
    sf::FloatRect nextTextBounds = nextButtonText.getLocalBounds();
    nextButtonText.setOrigin(nextTextBounds.width / 2, nextTextBounds.height / 2);
    nextButtonText.setPosition(windowWidth / 2, 550 + 10);
}


void GameGUI::run() {
    while (window.isOpen()) {
        handleEvents();
        render();

        // אם אנחנו במסך המשחק ורק שחקן אחד נשאר
        if (inGameScreen && !game.isGameActive()) {  // שימוש ב־gameActive דרך פונקציה או ישירות אם public
            try {
                std::string winnerName = game.getWinner();  // מחזיר שם של שחקן
                if (!winnerName.empty()) {
                    std::string message = "Game Over! Winner: " + winnerName;
                    showMessageToPlayer(message);
                } else {
                    showMessageToPlayer("Game Over! No winner.");
                }
            } catch (const std::exception& e) {
                showMessageToPlayer("Game Over! No winner.");
            }

            // מחזירים למסך הבית
            inGameScreen = false;
            gameStarted = false;
            readyToStartGame = false;
            playerListDisplay.clear();
        }
    }
}



void GameGUI::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (!gameStarted && event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            if (startButton.getGlobalBounds().contains(mousePos)) {
                gameStarted = true;
                game.startGame();
            }
        }

        if (gameStarted && event.type == sf::Event::TextEntered) {
            handleTextInput(event);
        }

        if (gameStarted && event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            if (addPlayerButton.getGlobalBounds().contains(mousePos)) {
                if (!playerNameInput.empty()) {
                    Player *p = PlayerFactory::createRandomPlayer(game, playerNameInput);
                    game.addPlayer(p);

                    sf::Text newPlayerText;
                    newPlayerText.setFont(font);
                    newPlayerText.setCharacterSize(20);
                    newPlayerText.setFillColor(sf::Color::White);
                    newPlayerText.setPosition(250, 420 + playerListDisplay.size() * 30);
                    newPlayerText.setString(playerNameInput + " (" + p->getRole() + ")");
                    playerListDisplay.push_back(newPlayerText);

                    if (game.players().size() == game.getNumPlayers()) {
                        readyToStartGame = true;
                    }

                    playerNameInput.clear();
                    inputText.setString("");
                }
            }

            if (readyToStartGame && nextButton.getGlobalBounds().contains(mousePos)) {
                std::cout << "Moving to the game..." << std::endl;
                inGameScreen = true;
            }
        }
    }
}

void GameGUI::showPlayerActionMenu(sf::RenderWindow &window, CoupG::Player &player) {
    sf::Font font;
    font.loadFromFile("arial.ttf");

    float margin = 10;
    float menuWidth = window.getSize().x - 2 * margin;
    float menuHeight = 50;
    sf::Vector2f menuPosition(margin, margin);

    sf::RectangleShape menuBox(sf::Vector2f(menuWidth, menuHeight));
    menuBox.setFillColor(sf::Color(200, 100, 100));
    menuBox.setPosition(menuPosition);

    std::vector<std::string> actions = {"gather", "tax", "bribe", "arrest", "sanction", "coup"};

    bool isSpy = (dynamic_cast<Spy *>(&player) != nullptr);
    bool isBaron = (dynamic_cast<Baron *>(&player) != nullptr);

    if (isSpy) {
        actions.push_back("use spy ability");
    }
    if (isBaron) {
        actions.push_back("Invest");
    }

    std::vector<sf::Text> menuOptions;
    for (size_t i = 0; i < actions.size(); i++) {
        sf::Text option;
        option.setFont(font);
        option.setCharacterSize(16);
        option.setFillColor(sf::Color::Black);
        option.setString(actions[i]);
        option.setPosition(menuPosition.x + 10 + i * 120, menuPosition.y + 10);
        menuOptions.push_back(option);
    }

    window.draw(menuBox);
    for (auto &option : menuOptions) {
        window.draw(option);
    }
    window.display();

    bool selected = false;
    while (!selected && window.isOpen()) {
        sf::Event event;
        try {
        game.validateTurnStart(player);
    } catch (const std::runtime_error &e) {
        std::string msg = e.what();
        if (msg == "Player is not active") {
            showMessageToPlayer("Player is not active");
        } else if (msg == "Not your turn") {
            showMessageToPlayer("Not your turn");
        } else if (msg == "Player must perform coup with 10 or more coins") {
            showMessageToPlayer("You must perform a coup!");

            std::vector<Player *> targets;
            for (Player *p : game.getAllPlayers()) {
                if (p != &player && p->getActive()) {
                    targets.push_back(p);
                }
            }

            Player *target = chooseTargetPlayer(window, targets, font);
            if (target) {
                try {
                    player.coup(*target);

                    // בדיקה האם גנרל מונע את המהלך
                    for (Player *p : game.getAllPlayers()) {
                        if (p->getActive() && p != &player) {
                            if (General *gen = dynamic_cast<General *>(p)) {
                                bool cancelCoup = showYesNoPopup(window, font,
                                    gen->getName() + ", do you want to prevent a coup against " + target->getName() + "?");
                                if (cancelCoup) {
                                    gen->useAbility(*target);
                                    showMessageToPlayer("Coup was prevented by " + gen->getName());
                                    break;
                                }
                            }
                        }
                    }
                } catch (std::runtime_error &err) {
                    showMessageToPlayer("Coup failed: " + std::string(err.what()));
                }
            }

            return;
        }
        return;
    }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;

                if (mouseY >= menuPosition.y + 10 && mouseY <= menuPosition.y + 40) {
                    int index = (mouseX - static_cast<int>(menuPosition.x + 10)) / 120;
                    if (index >= 0 && index < actions.size()) {
                        std::string choice = actions[index];
                        if (choice == "gather") {
                            try {
                                player.gather();
                            } catch (const std::runtime_error &e) {
                                showMessageToPlayer("You are under a sanction.");
                            }

                        } else if (choice == "tax") {
                            try {
                                player.tax();
                            } catch (const std::runtime_error &e) {
                                showMessageToPlayer("You are under a sanction.");
                            }

                            for (Player *p : game.getAllPlayers()) {
                                if (p->getActive() && p != &player) {
                                    if (Governor *gov = dynamic_cast<Governor *>(p)) {
                                        bool cancelTax = showYesNoPopup(window, font,
                                            gov->getName() + ", do you want to block the Tax from " + player.getName() + "?");
                                        if (cancelTax) {
                                            gov->useAbility(player);
                                            showMessageToPlayer("Tax was blocked by " + gov->getName());
                                            break;
                                        }
                                    }
                                }
                            }

                        } else if (choice == "bribe") {
                            player.bribe();
                            for (Player *p : game.getAllPlayers()) {
                                if (p->getActive() && p != &player) {
                                    if (Judge *jud = dynamic_cast<Judge *>(p)) {
                                        bool cancelTax = showYesNoPopup(window, font,
                                            jud->getName() + ", do you want to block the bribe from " + player.getName() + "?");
                                        if (cancelTax) {
                                            jud->useAbility(player);
                                            showMessageToPlayer("bribe was blocked by " + jud->getName());
                                            break;
                                        }
                                    }
                                }
                            }

                        } else if (choice == "arrest" || choice == "sanction" || choice == "coup") {
                            std::vector<Player *> targets;
                            for (Player *p : game.getAllPlayers()) {
                                if (p != &player && p->getActive()) {
                                    targets.push_back(p);
                                }
                            }
                            Player *target = chooseTargetPlayer(window, targets, font);
                            if (target) {
                                if (choice == "arrest") {
                                    try {
                                        player.arrest(*target);
                                    } catch (const std::runtime_error &e) {
                                        std::string msg=e.what();
                                        if (msg == "1") {

                                            showMessageToPlayer("You are under a block arrest");
                                        }
                                        else if (msg == "2") {
                                                showMessageToPlayer(target->getName()+" doesn't have enough money");
                                            }
                                        else if (msg == "3"){
                                            showMessageToPlayer(target->getName()+" can't be arrested again");
                                        }
                                    }
                                } else if (choice == "sanction") {
                                    try {
                                        player.sanction(*target);
                                    }
                                    catch (std::runtime_error &e) {
                                        showMessageToPlayer("You don't have enough money to sunction.");
                                    }

                                } else if (choice == "coup") {
                                    try {
                                        player.coup(*target);
                                        for (Player *p : game.getAllPlayers()) {
                                            if (p->getActive() && p != &player) {
                                                if (General *gen = dynamic_cast<General *>(p)) {
                                                    bool cancelTax = showYesNoPopup(window, font,
                                                        gen->getName() + ", do you want to prevent a coup against " + target->getName() + "?");
                                                    if (cancelTax) {
                                                        try {
                                                            gen->useAbility(*target);
                                                            showMessageToPlayer("coup was prevent by " + gen->getName());
                                                            break;
                                                        }
                                                        catch (std::runtime_error &e) {
                                                            showMessageToPlayer("You don't have enough money to prevent coup");
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    catch (std::runtime_error &e) {
                                        showMessageToPlayer("You don't have enough money to coup.");

                                    }

                                }
                            }

                        } else if (choice == "Invest") {
                            try {
                                dynamic_cast<Baron &>(player).useAbility();
                            } catch (const std::runtime_error &e) {
                                showMessageToPlayer("You don't have enough money to invest.");
                            }

                        } else if (choice == "use spy ability" && isSpy) {
                            std::vector<Player *> targets;
                            for (Player *p : game.getAllPlayers()) {
                                if (p != &player && p->getActive()) {
                                    targets.push_back(p);
                                }
                            }
                            Player *target = chooseTargetPlayer(window, targets, font);
                            if (target) {
                                int coins = dynamic_cast<Spy &>(player).useAbility(*target);
                                showMessageToPlayer("Target has " + std::to_string(coins) + " coins");


                                window.draw(menuBox);
                                for (auto &option : menuOptions) window.draw(option);
                                window.display();
                                sleep(sf::seconds(2));
                            }
                        }

                        selected = true;

                    }

                }
            }
        }
    }
}



void GameGUI::showMessageToPlayer(const std::string &message) {
    sf::RenderWindow window(sf::VideoMode(600, 150), "הודעה לשחקן", sf::Style::Titlebar | sf::Style::Close);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return; // טיפול במקרה שהפונט לא נטען
    }

    sf::Text text;
    text.setFont(font);
    text.setString(message);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);

    // מיקום הטקסט במרכז החלון
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // סגור את החלון אחרי 3 שניות
        if (clock.getElapsedTime().asSeconds() >= 3.0f) {
            window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(text);
        window.display();
    }
}


bool GameGUI::showYesNoPopup(sf::RenderWindow &window, sf::Font &font, const std::string &message) {
    sf::RenderWindow popup(sf::VideoMode(500, 200), "Confirmation", sf::Style::Titlebar | sf::Style::Close);

    sf::Text msgText;
    msgText.setFillColor(sf::Color::White);
    msgText.setFont(font);
    msgText.setString(message);
    msgText.setCharacterSize(20);
    msgText.setFillColor(sf::Color::White);
    msgText.setPosition(50, 50);

    sf::RectangleShape yesButton(sf::Vector2f(100, 40));
    sf::Color softGreen(100, 200, 100);
    yesButton.setFillColor(softGreen);
    yesButton.setPosition(100, 120);

    sf::RectangleShape noButton(sf::Vector2f(100, 40));
    noButton.setFillColor(sf::Color(200, 100, 100));
    noButton.setPosition(300, 120);

    sf::Text yesText("Yes", font, 20);
    yesText.setFillColor(sf::Color::White);
    yesText.setPosition(130, 125);

    sf::Text noText("No", font, 20);
    noText.setFillColor(sf::Color::White);
    noText.setPosition(335, 125);

    while (popup.isOpen()) {
        sf::Event event;
        while (popup.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                popup.close();
                return false;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
                if (yesButton.getGlobalBounds().contains(mouse)) {
                    popup.close();
                    return true;
                }
                if (noButton.getGlobalBounds().contains(mouse)) {
                    popup.close();
                    return false;
                }
            }
        }

        popup.clear(sf::Color::Black);
        popup.draw(msgText);
        popup.draw(yesButton);
        popup.draw(noButton);
        popup.draw(yesText);
        popup.draw(noText);
        popup.display();
    }

    return false;
}


Player *GameGUI::chooseTargetPlayer(sf::RenderWindow &window, std::vector<Player *> players, sf::Font &font) {
    sf::RectangleShape menuBox(sf::Vector2f(250, 30 * players.size()));
    menuBox.setFillColor(sf::Color(200, 100, 100));
    menuBox.setPosition(500, 100);

    std::vector<sf::Text> playerOptions;
    for (size_t i = 0; i < players.size(); ++i) {
        sf::Text txt;
        txt.setFont(font);
        txt.setCharacterSize(16);
        txt.setFillColor(sf::Color::Black);
        txt.setString(players[i]->getName());
        txt.setPosition(510, 100 + i * 30);
        playerOptions.push_back(txt);
    }

    window.draw(menuBox);
    for (auto &txt: playerOptions)
        window.draw(txt);
    window.display();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                int mouseY = event.mouseButton.y;
                int index = (mouseY - 100) / 30;
                if (index >= 0 && index < players.size()) {
                    return players[index];
                }
            }
        }
    }
    return nullptr;
}

void GameGUI::renderGameBoard() {
    const float windowWidth = 1000;
    const float windowHeight = 600;
    const float centerX = windowWidth / 2;
    const float centerY = windowHeight / 2;
    const float radius = 200;

    auto players = game.getAllPlayers();
    size_t n = players.size();
    size_t curr = game.getCurrentPlayer();

    // טקסט על תור נוכחי - ממורכז למעלה
    currentTurnText.setFont(font);
    currentTurnText.setCharacterSize(24);
    currentTurnText.setFillColor(sf::Color::White);
    currentTurnText.setString("Current Turn: " + players[curr]->getName());
    sf::FloatRect textBounds = currentTurnText.getLocalBounds();
    currentTurnText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    currentTurnText.setPosition(centerX, 30);
    window.draw(currentTurnText);

    for (int i = 0; i < n; ++i) {
        float angle = 2 * M_PI * i / n;
        float x = centerX + radius * std::cos(angle);
        float y = centerY + radius * std::sin(angle);

        // שחקן כעיגול
        sf::CircleShape circle(30);
        circle.setPosition(x - 30, y - 30);
        sf::Color softGreen(100, 200, 100);
        circle.setFillColor(players[i]->getActive() ? softGreen : sf::Color::Red);
        window.draw(circle);

        // עיגול צהוב לשחקן הנוכחי
        if (i == curr) {
            sf::CircleShape outline(34);
            outline.setPosition(x - 34, y - 34);
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineThickness(4);
            outline.setOutlineColor(sf::Color(200, 100, 100));
            window.draw(outline);
        }

        // שם
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setCharacterSize(15);
        nameText.setFillColor(sf::Color::White);
        nameText.setString("Name: " + players[i]->getName());
        sf::FloatRect nameBounds = nameText.getLocalBounds();
        nameText.setOrigin(nameBounds.width / 2, 0);
        nameText.setPosition(x, y + 35);
        window.draw(nameText);

        // תפקיד
        sf::Text roleText;
        roleText.setFont(font);
        roleText.setCharacterSize(15);
        roleText.setFillColor(sf::Color::White);
        roleText.setString("Role: " + players[i]->getRole());
        sf::FloatRect roleBounds = roleText.getLocalBounds();
        roleText.setOrigin(roleBounds.width / 2, 0);
        roleText.setPosition(x, y + 55);
        window.draw(roleText);

        // מטבעות
        sf::Text coinsText;
        coinsText.setFont(font);
        coinsText.setCharacterSize(15);
        coinsText.setFillColor(sf::Color(200, 100, 100));
        coinsText.setString("Coins: " + std::to_string(players[i]->getCoins()));
        sf::FloatRect coinBounds = coinsText.getLocalBounds();
        coinsText.setOrigin(coinBounds.width / 2, 0);
        coinsText.setPosition(x, y + 70);
        window.draw(coinsText);
    }
}


void GameGUI::handleTextInput(sf::Event &event) {
    if (event.text.unicode == 8) {
        if (!playerNameInput.empty())
            playerNameInput.pop_back();
    } else if (event.text.unicode < 128 && std::isprint(event.text.unicode)) {
        playerNameInput += static_cast<char>(event.text.unicode);
    }
    inputText.setString(playerNameInput);
}

void GameGUI::drawPlayerList() {
    for (auto &playerText: playerListDisplay) {
        window.draw(playerText);
    }
}

void GameGUI::render() {
    window.clear(sf::Color::Black);

    if (!gameStarted) {
        window.draw(title);
        window.draw(startButton);
        window.draw(startText);
    } else {
        if (inGameScreen) {
            renderGameBoard();
            std::string currentPlayerName = game.turn();
            for (Player *p: game.getAllPlayers()) {
                if (p->getName() == currentPlayerName && p->getActive()) {
                    showPlayerActionMenu(window, *p);
                    break;
                }
            }
        } else {
            window.draw(title);
            window.draw(inputBox);
            window.draw(inputText);
            window.draw(addPlayerButton);
            window.draw(addPlayerText);
            drawPlayerList();
            if (readyToStartGame) {
                window.draw(nextButton);
                window.draw(nextButtonText);
            }
        }
    }

    window.display();
}
