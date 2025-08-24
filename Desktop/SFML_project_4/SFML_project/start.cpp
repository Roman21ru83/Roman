#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>

int main() {
    // Create a window for the start menu
    sf::RenderWindow window(sf::VideoMode(640, 480), "Game Start Menu with Characters");

    // Load font used in menu texts
    sf::Font font;
    if (!font.loadFromFile("fonts/OpenSans-Regular.ttf")) {
        std::cerr << "Failed to load font!\n";
        return 1;
    }

    // Load character textures
    sf::Texture warriorTexture, mageTexture, archerTexture;
    if (!warriorTexture.loadFromFile("textures/warrior.png") ||
        !mageTexture.loadFromFile("textures/mage.png") ||
        !archerTexture.loadFromFile("textures/archer.png")) {
        std::cerr << "Failed to load one or more character textures!\n";
        return 1;
    }

    // Define main menu options
    std::vector<std::string> mainMenu = {"Start Game", "Quit Game"};
    int mainMenuIndex = 0;
    bool inCharacterSelect = false;

    // Define available character names
    std::vector<std::string> characters = {"Warrior", "Mage", "Archer"};
    std::vector<sf::Sprite> characterSprites(3);

    // Assign textures to character sprites
    characterSprites[0].setTexture(warriorTexture);
    characterSprites[1].setTexture(mageTexture);
    characterSprites[2].setTexture(archerTexture);

    // Set position and scale for character preview sprites
    for (int i = 0; i < 3; ++i) {
        characterSprites[i].setPosition(400.f, 140.f + i * 60.f);
        characterSprites[i].setScale(2.f, 2.f); // Enlarge sprite
    }

    // Create text for main menu items
    std::vector<sf::Text> mainMenuTexts(2);
    for (int i = 0; i < 2; ++i) {
        mainMenuTexts[i].setFont(font);
        mainMenuTexts[i].setString(mainMenu[i]);
        mainMenuTexts[i].setCharacterSize(32);
        mainMenuTexts[i].setPosition(200.f, 180.f + i * 50.f);
    }

    // Create text for character names
    std::vector<sf::Text> characterTexts(3);
    for (int i = 0; i < 3; ++i) {
        characterTexts[i].setFont(font);
        characterTexts[i].setString(characters[i]);
        characterTexts[i].setCharacterSize(28);
        characterTexts[i].setPosition(100.f, 150.f + i * 60.f);
    }

    int characterIndex = 0; // Index of currently selected character

    // Main menu loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (!inCharacterSelect) {
                    // Navigate main menu using arrow keys
                    if (event.key.code == sf::Keyboard::Up)
                        mainMenuIndex = (mainMenuIndex + mainMenu.size() - 1) % mainMenu.size();
                    if (event.key.code == sf::Keyboard::Down)
                        mainMenuIndex = (mainMenuIndex + 1) % mainMenu.size();
                    if (event.key.code == sf::Keyboard::Enter) {
                        // Handle menu selection
                        if (mainMenuIndex == 0) {
                            inCharacterSelect = true; // Proceed to character selection
                        } else {
                            window.close(); // Quit game
                        }
                    }
                } else {
                    // Navigate character list
                    if (event.key.code == sf::Keyboard::Up)
                        characterIndex = (characterIndex + characters.size() - 1) % characters.size();
                    if (event.key.code == sf::Keyboard::Down)
                        characterIndex = (characterIndex + 1) % characters.size();
                    if (event.key.code == sf::Keyboard::Enter) {
                        // Save selected character to file
                        std::ofstream file("selected_character.txt");
                        if (file.is_open()) {
                            file << characters[characterIndex];
                            file.close();
                        }

                        window.close();

                        // Launch main game (make sure the path and name match your compiled file)
                        system("./main-app"); // Launch maze game
                    }
                }
            }
        }

        // Draw UI
        window.clear(sf::Color::Black);

        if (!inCharacterSelect) {
            // Draw main menu options
            for (int i = 0; i < 2; ++i) {
                mainMenuTexts[i].setFillColor(i == mainMenuIndex ? sf::Color::Yellow : sf::Color::White);
                window.draw(mainMenuTexts[i]);
            }
        } else {
            // Draw character options with highlight and sprites
            for (int i = 0; i < 3; ++i) {
                characterTexts[i].setFillColor(i == characterIndex ? sf::Color::Green : sf::Color::White);
                window.draw(characterTexts[i]);
                window.draw(characterSprites[i]);
            }
        }

        window.display();
    }

    return 0;
}
