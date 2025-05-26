#include "GameGUI.h"
#include <iostream>
#include <cmath>
#include <SFML/System.hpp>
#include "CoupGame/Roles/Baron.h"
#include "CoupGame/ActionType.h"
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
    sf::Color goldColor(218, 165, 32);
    sf::Color brownColor(94, 63, 36);
    sf::Color goldHighlight(255, 215, 0);
    startButton.setFillColor(brownColor);
    startButton.setOutlineThickness(3);
    startButton.setOutlineColor(goldColor);
    startButton.setPosition((windowWidth - startButton.getSize().x) / 2, 200);

    startText.setFont(font);
    startText.setString("START GAME");
    startText.setCharacterSize(24);
    startText.setFillColor(goldHighlight);
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
    addPlayerButton.setFillColor(brownColor);
    addPlayerButton.setOutlineThickness(3);
    addPlayerButton.setOutlineColor(goldColor);
    addPlayerButton.setPosition((windowWidth - addPlayerButton.getSize().x) / 2, 360);

    addPlayerText.setFont(font);
    addPlayerText.setString("Add Player");
    addPlayerText.setCharacterSize(20);
    addPlayerText.setFillColor(goldHighlight);
    sf::FloatRect addPlayerTextBounds = addPlayerText.getLocalBounds();
    addPlayerText.setOrigin(addPlayerTextBounds.width / 2, addPlayerTextBounds.height / 2);
    addPlayerText.setPosition(windowWidth / 2, 368 + 8); // יישור אנכי בתוך הכפתור

    // Next Button
    nextButton.setSize(sf::Vector2f(200, 50));
    nextButton.setFillColor(brownColor);
    nextButton.setOutlineThickness(3);
    nextButton.setOutlineColor(goldColor);
    nextButton.setPosition((windowWidth - nextButton.getSize().x) / 2, 540);

    nextButtonText.setFont(font);
    nextButtonText.setString("Next To The Game");
    nextButtonText.setCharacterSize(22);
    nextButtonText.setFillColor(goldHighlight);
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
                    try {
                        game.addPlayer(p);
                    }
                    catch (const std::exception& e) {
                        delete p;
                    }

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
    if (!font.loadFromFile("arial.ttf")) {
        // טיפול במקרה שהפונט לא נטען
        std::cerr << "Failed to load font arial.ttf" << std::endl;
        return;
    }

    float margin = 10.f;
    float buttonWidth = 140.f;
    float buttonHeight = 40.f;
    float menuX = margin;
    float menuY = 100.f; // מתחילים מלמעלה, מתחת לכותרת
    float buttonSpacing = 10.f;

    std::vector<std::string> actions = {"gather", "tax", "bribe", "arrest", "sanction", "coup"};
    if (player.getRole() == "Spy") {
        actions.push_back("use spy ability");
    }
    if (player.getRole() == "Baron") {
        actions.push_back("Invest");
    }

    // צבעים בגווני זהב וחום
    sf::Color goldColor(218, 165, 32);
    sf::Color brownColor(94, 63, 36);
    sf::Color goldHighlight(255, 215, 0);

    // יצירת תיבת תפריט כללית (menuBox) שתהיה רקע אחיד שמאחורי הכפתורים
    sf::RectangleShape menuBox(sf::Vector2f(buttonWidth + 2 * margin, (buttonHeight + buttonSpacing) * actions.size() - buttonSpacing + 2 * margin));
    menuBox.setPosition(menuX - margin, menuY - margin);
    menuBox.setFillColor(sf::Color(50, 30, 10, 230)); // חצי שקוף חום כהה
    menuBox.setOutlineThickness(2);
    menuBox.setOutlineColor(goldColor);

    // יצירת כפתורים וטקסטים
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> buttonTexts;

    for (size_t i = 0; i < actions.size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
        button.setPosition(menuX, menuY + i * (buttonHeight + buttonSpacing));
        button.setFillColor(brownColor);
        button.setOutlineThickness(3);
        button.setOutlineColor(goldColor);
        buttons.push_back(button);

        sf::Text text;
        text.setFont(font);
        text.setString(actions[i]);
        text.setCharacterSize(18);
        text.setFillColor(goldHighlight);

        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
        text.setPosition(menuX + buttonWidth / 2, menuY + i * (buttonHeight + buttonSpacing) + buttonHeight / 2);
        buttonTexts.push_back(text);
    }

    window.clear();
    renderGameBoard();

    // מציירים את תיבת התפריט והרכיבים
    window.draw(menuBox);
    for (size_t i = 0; i < buttons.size(); ++i) {
        window.draw(buttons[i]);
        window.draw(buttonTexts[i]);
    }
    window.display();

    bool selected = false;

    // לולאת אירועים
    while (!selected && window.isOpen()) {
        sf::Event event;

        // בודקים תקינות תור
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

                        for (Player *p : game.getAllPlayers()) {
                            if (p->getActive() && p != &player && p->getRole() == "General") {
                                bool cancelCoup = showYesNoPopup(window, font,
                                    p->getName() + ", do you want to prevent a coup against " + target->getName() + "?");
                                if (cancelCoup) {
                                    p->useAbility(*target);
                                    showMessageToPlayer("Coup was prevented by " + p->getName());
                                    break;
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

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;

                for (size_t i = 0; i < buttons.size(); ++i) {
                    if (buttons[i].getGlobalBounds().contains(static_cast<float>(mouseX), static_cast<float>(mouseY))) {
                        std::string choice = actions[i];

                        if (choice == "gather") {
                            try {
                                player.gather();
                            } catch (const std::runtime_error &) {
                                showMessageToPlayer("You are under a sanction.");
                            }

                        } else if (choice == "tax") {
                            try {
                                player.tax();
                            } catch (const std::runtime_error &) {
                                showMessageToPlayer("You are under a sanction.");
                            }

                            for (Player *p : game.getAllPlayers()) {
                                if (p->getActive() && p != &player && p->getRole() == "Governor") {
                                    bool cancelTax = showYesNoPopup(window, font,
                                        p->getName() + ", do you want to block the Tax from " + player.getName() + "?");
                                    if (cancelTax) {
                                        p->useAbility(player);
                                        showMessageToPlayer("Tax was blocked by " + p->getName());
                                        break;
                                    }
                                }
                            }

                        } else if (choice == "bribe") {
                            player.bribe();
                            for (Player *p : game.getAllPlayers()) {
                                if (p->getActive() && p != &player && p->getRole() == "Judge") {
                                    bool cancelBribe = showYesNoPopup(window, font,
                                        p->getName() + ", do you want to block the bribe from " + player.getName() + "?");
                                    if (cancelBribe) {
                                        p->useAbility(player);
                                        showMessageToPlayer("Bribe was blocked by " + p->getName());
                                        break;
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
                                        std::string msg = e.what();
                                        if (msg == "1") {
                                            showMessageToPlayer("You are under a block arrest");
                                        } else if (msg == "2") {
                                            showMessageToPlayer(target->getName() + " doesn't have enough money");
                                        } else if (msg == "3") {
                                            showMessageToPlayer(target->getName() + " can't be arrested again");
                                        }
                                    }
                                } else if (choice == "sanction") {
                                    try {
                                        player.sanction(*target);
                                    } catch (std::runtime_error &) {
                                        showMessageToPlayer("You don't have enough money to sanction.");
                                    }
                                } else if (choice == "coup") {
                                    try {
                                        player.coup(*target);

                                        for (Player *p : game.getAllPlayers()) {
                                            if (p->getActive() && p != &player && p->getRole() == "General") {
                                                bool cancelCoup = showYesNoPopup(window, font,
                                                    p->getName() + ", do you want to prevent a coup against " + target->getName() + "?");
                                                if (cancelCoup) {
                                                    try {
                                                        p->useAbility(*target);
                                                        showMessageToPlayer("Coup was prevented by " + p->getName());
                                                        break;
                                                    } catch (std::runtime_error &) {
                                                        showMessageToPlayer("You don't have enough money to prevent coup");
                                                    }
                                                }
                                            }
                                        }
                                    } catch (std::runtime_error &) {
                                        showMessageToPlayer("You don't have enough money to coup.");
                                    }
                                }
                            }

                        } else if (choice == "Invest" && player.getRole() == "Baron") {
                            try {
                                player.useAbility();
                            } catch (const std::runtime_error &) {
                                showMessageToPlayer("You don't have enough money to invest.");
                            }

                        } else if (choice == "use spy ability" && player.getRole() == "Spy") {
                            std::vector<Player *> targets;
                            for (Player *p : game.getAllPlayers()) {
                                if (p != &player && p->getActive()) {
                                    targets.push_back(p);
                                }
                            }
                            Player *target = chooseTargetPlayer(window, targets, font);
                            if (target) {
                                int coins = player.useSpyAbility(*target);
                                showMessageToPlayer("Target has " + std::to_string(coins) + " coins");

                                // הדגמה של חלון קטן עם ההודעה
                                sf::RectangleShape infoBox(sf::Vector2f(300.f, 100.f));
                                infoBox.setFillColor(sf::Color(50, 30, 10, 230));
                                infoBox.setOutlineThickness(2);
                                infoBox.setOutlineColor(goldColor);
                                infoBox.setPosition((window.getSize().x - 300) / 2.f, (window.getSize().y - 100) / 2.f);

                                sf::Text infoText;
                                infoText.setFont(font);
                                infoText.setString("Target has " + std::to_string(coins) + " coins");
                                infoText.setCharacterSize(20);
                                infoText.setFillColor(goldHighlight);
                                sf::FloatRect bounds = infoText.getLocalBounds();
                                infoText.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
                                infoText.setPosition(infoBox.getPosition().x + 150.f, infoBox.getPosition().y + 50.f);

                                window.clear();
                                renderGameBoard();
                                window.draw(menuBox);
                                for (size_t j = 0; j < buttons.size(); ++j) {
                                    window.draw(buttons[j]);
                                    window.draw(buttonTexts[j]);
                                }
                                window.draw(infoBox);
                                window.draw(infoText);
                                window.display();

                                sf::sleep(sf::seconds(2)); // מציגים את ההודעה 2 שניות
                            }
                        }

                        selected = true;
                        break;
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
    // צבעים בגווני זהב וחום:
    // זהב: (212, 175, 55)
    // חום כהה: (101, 67, 33)

    sf::RectangleShape menuBox(sf::Vector2f(250, 30 * players.size()));
    menuBox.setFillColor(sf::Color(212, 175, 55));  // זהב
    menuBox.setPosition(500, 100);

    std::vector<sf::Text> playerOptions;
    for (size_t i = 0; i < players.size(); ++i) {
        sf::Text txt;
        txt.setFont(font);
        txt.setCharacterSize(16);
        txt.setFillColor(sf::Color(101, 67, 33));  // חום כהה
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

std::string actionTypeToString(CoupG::ActionType action) {
    switch (action) {
        case ActionType::None: return "None";
        case ActionType::Gather: return "Gather";
        case ActionType::Tax: return "Tax";
        case ActionType::Bribe: return "Bribe";
        case ActionType::Arrest: return "Arrest";
        case ActionType::Sanction: return "Sanction";
        case ActionType::Coup: return "Coup";
        case ActionType::Invest: return "Invest";
        case ActionType::Spy: return "Spy";
        case ActionType::UndoTax: return "UndoTax";
        default: return "Unknown";
    }
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
    currentTurnText.setPosition(centerX+300, 30);
    window.draw(currentTurnText);

    // שולחן עגול עם הצללה וטקסט
    sf::CircleShape tableShadow(radius - 30);  // צללה טיפה יותר גדולה
    tableShadow.setFillColor(sf::Color(30, 30, 30, 150));  // אפור כהה עם שקיפות
    tableShadow.setOrigin(tableShadow.getRadius(), tableShadow.getRadius());
    tableShadow.setPosition(centerX + 5, centerY + 5);  // מעט מוזז למטה-ימינה
    window.draw(tableShadow);

    sf::CircleShape table(radius + 60); // טיפה יותר גדול
    table.setFillColor(sf::Color(94, 63, 36, 100)); // חום כהה עם שקיפות (100 מתוך 255)
    table.setOutlineColor(sf::Color(218, 165, 32, 150)); // זהב עם שקיפות 150
    table.setOutlineThickness(6);
    table.setOrigin(table.getRadius(), table.getRadius());
    table.setPosition(centerX, centerY);
    window.draw(table);


    // טקסט במרכז השולחן
    sf::Text tableText;
    tableText.setFont(font);
    tableText.setCharacterSize(28);
    tableText.setStyle(sf::Text::Bold | sf::Text::Italic);
    tableText.setFillColor(sf::Color(255, 215, 0,100)); // זהב נוצץ
    tableText.setString("COUP TABLE");
    sf::FloatRect tableTextBounds = tableText.getLocalBounds();
    tableText.setOrigin(tableTextBounds.width / 2, tableTextBounds.height / 2);
    tableText.setPosition(centerX, centerY - 10);
    window.draw(tableText);

    sf::Color activePlayerColor(0, 168, 232);     // טורקיז כחול יפה
    sf::Color inactivePlayerColor(120, 120, 120); // אפור מעודן


    for (int i = 0; i < n; ++i) {
        float angle = 2 * M_PI * i / n;
        float x = centerX + radius * std::cos(angle);
        float y = centerY + radius * std::sin(angle);

        // שחקן כעיגול
        sf::CircleShape circle(30);
        circle.setPosition(x - 30, y - 30);
        sf::Color softGreen(100, 200, 100);
        circle.setFillColor(players[i]->getActive() ? activePlayerColor : inactivePlayerColor);
        window.draw(circle);

        // עיגול צהוב לשחקן הנוכחי
        if (i == curr) {
            sf::CircleShape outline(34);
            outline.setPosition(x - 34, y - 34);
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineThickness(4);
            outline.setOutlineColor(sf::Color(255, 215, 0)); // ורוד זוהר / סגול
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

        // פעולה אחרונה
        sf::Text actionText;
        actionText.setFont(font);
        actionText.setCharacterSize(15);
        actionText.setFillColor(sf::Color::White);
        actionText.setString("Action: " + actionTypeToString(players[i]->getLastAction()));
        sf::FloatRect actionBounds = actionText.getLocalBounds();
        actionText.setOrigin(actionBounds.width / 2, 0);
        actionText.setPosition(x, y + 85);  // מתחת למטבעות
        window.draw(actionText);


        // מטבעות
        sf::Text coinsText;
        coinsText.setFont(font);
        coinsText.setCharacterSize(15);
        coinsText.setFillColor(sf::Color::White);
        if (i == curr) {
            coinsText.setString("Coins: " + std::to_string(players[i]->getCoins()));
        } else {
            coinsText.setString("Coins: ???");
        }
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

int main() {
    CoupG::GameGUI gui(4);
    gui.run();
    return 0;
}
