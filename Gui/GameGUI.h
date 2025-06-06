//Email:Edenhassin@gmail.com

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../CoupGame/Game.h"
#include "../CoupGame/PlayerFactory.h"

namespace CoupG {

    class GameGUI {
    public:
        explicit GameGUI(int numPlayers);
        void run();

    private:

        int numPlayers = 0;
        bool playersSelected = false;
        std::vector<sf::Text> playerCountOptions;
        sf::RectangleShape dropdownBox;
        bool dropdownVisible = false;
        sf::Text selectedPlayerCountText;


        sf::RenderWindow window;
        sf::Font font;
        sf::Text title;

        sf::RectangleShape startButton;
        sf::Text startText;

        sf::RectangleShape inputBox;
        sf::Text inputText;
        std::string playerNameInput;

        sf::RectangleShape addPlayerButton;
        sf::Text addPlayerText;

        std::vector<sf::Text> playerListDisplay;

        sf::RectangleShape nextButton;
        sf::Text nextButtonText;
        bool readyToStartGame = false;

        sf::Text currentTurnText;
        bool inGameScreen = false;

        bool gameStarted = false;
        Game game;

        void handleEvents();
        void render();
        void handleTextInput(const sf::Event& event);
        void drawPlayerList();
        void renderGameBoard();
        void showMessageToPlayer(const std::string& message);
        bool showYesNoPopup(sf::Font &font, const std::string &message);
        Player *chooseTargetPlayer(sf::RenderWindow &window, std::vector<Player *> players, sf::Font &font);
        void showPlayerActionMenu(sf::RenderWindow &window, Player &player);
        void handlePlayerActionClick(const std::string &choice,Player &player, sf::RenderWindow &window, sf::Font &font);
        bool validatePlayerTurnStart(Player &player, sf::RenderWindow &window, sf::Font &font);
    };
}
