#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// Structure to store enemy data
struct Enemy {
    int type;                  // Enemy type ID
    int hp;                    // Current HP
    sf::Vector2f position;     // Enemy's position on the map
};

// Game constants
const int TILE_SIZE = 32;
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;
const float PLAYER_SPEED = 100.0f;

// Player statistics
int playerHP = 0;
int playerAttack = 0;
int playerDefense = 0;
int playerMaxHP = 0;

// Returns starting HP based on enemy type
int getInitialHP(int type) {
    switch (type) {
        case 3: return 40;   // Goblin
        case 4: return 60;   // Skeleton
        case 5: return 90;   // Demon
        case 9: return 200;  // Final Boss
        default: return 0;
    }
}

// Map layout legend:
// 0 = Floor
// 1 = Wall
// 2 = Boss Room
// 3 = Goblin
// 4 = Skeleton
// 5 = Demon
// 6 = Heal pickup
// 9 = Final Boss
std::vector<std::vector<int>> map = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,3,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,0,1},
    {1,6,1,0,1,0,1,3,0,0,1,0,1,0,1,3,0,1,0,1},
    {1,0,0,0,1,0,1,6,1,0,1,0,1,0,1,6,0,1,0,1},
    {1,0,0,0,1,0,1,1,1,0,1,0,1,0,1,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,1,0,1,0,1},
    {1,0,1,4,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,1,6,1,5,0,0,0,0,0,0,0,1,0,1,0,1,0,1},
    {1,0,1,1,1,6,1,1,1,1,1,1,0,1,0,1,0,1,0,1},
    {1,0,0,0,0,0,1,9,2,2,2,1,0,0,0,1,0,1,0,1},
    {1,1,1,1,1,1,1,2,2,2,2,1,1,1,1,1,0,1,0,1},
    {1,0,9,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

int main() {
    // Create game window
    sf::RenderWindow window(sf::VideoMode(MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE), "Maze Game");

    // Load font for UI
    sf::Font font;
    font.loadFromFile("fonts/OpenSans-Regular.ttf");

    // Set up HP text display
    sf::Text hpText;
    hpText.setFont(font);
    hpText.setCharacterSize(18);
    hpText.setPosition(10.f, 5.f);

    // HP bar UI
    sf::RectangleShape hpBarBack(sf::Vector2f(100, 10));
    hpBarBack.setFillColor(sf::Color(50, 50, 50));
    hpBarBack.setPosition(10.f, 30.f);

    sf::RectangleShape hpBarFront(sf::Vector2f(100, 10));
    hpBarFront.setFillColor(sf::Color::Red);
    hpBarFront.setPosition(10.f, 30.f);

    // Load textures for map and characters
    sf::Texture floorTexture, wallTexture, healTexture;
    floorTexture.loadFromFile("textures/floor.png");
    wallTexture.loadFromFile("textures/wall.png");
    healTexture.loadFromFile("textures/heal.png");

    sf::Texture goblinTexture, skeletonTexture, demonTexture, bossTexture;
    goblinTexture.loadFromFile("textures/goblin.png");
    skeletonTexture.loadFromFile("textures/skeleton.png");
    demonTexture.loadFromFile("textures/demon.png");
    bossTexture.loadFromFile("textures/boss.png");

    // Read selected character from file
    std::string selectedCharacter;
    std::ifstream file("selected_character.txt");
    std::getline(file, selectedCharacter);
    file.close();

    // Load player texture and stats
    sf::Texture playerTexture;
    if (selectedCharacter == "Warrior") {
        playerTexture.loadFromFile("textures/warrior.png");
        playerHP = playerMaxHP = 150;
        playerAttack = 20;
        playerDefense = 10;
    } else if (selectedCharacter == "Mage") {
        playerTexture.loadFromFile("textures/mage.png");
        playerHP = playerMaxHP = 100;
        playerAttack = 35;
        playerDefense = 5;
    } else {
        playerTexture.loadFromFile("textures/archer.png");
        playerHP = playerMaxHP = 120;
        playerAttack = 25;
        playerDefense = 8;
    }

    // Initialize player sprite and position
    sf::RectangleShape player(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    player.setTexture(&playerTexture);
    sf::Vector2f playerPosition(1 * TILE_SIZE, 1 * TILE_SIZE);

    sf::Clock clock;

    // Initialize enemies based on map tiles
    std::vector<Enemy> enemies;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int type = map[y][x];
            if (type == 3 || type == 4 || type == 5 || type == 9) {
                Enemy enemy;
                enemy.type = type;
                enemy.hp = getInitialHP(type);
                enemy.position = sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE);
                enemies.push_back(enemy);
            }
        }
    }

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();
        sf::Vector2f newPosition = playerPosition;

        // Handle player movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) newPosition.y -= PLAYER_SPEED * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) newPosition.y += PLAYER_SPEED * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) newPosition.x -= PLAYER_SPEED * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) newPosition.x += PLAYER_SPEED * deltaTime;

        // Movement collision check
        int tileX = newPosition.x / TILE_SIZE;
        int tileY = newPosition.y / TILE_SIZE;
        if (map[tileY][tileX] != 1) {
            playerPosition = newPosition;
        }

        // Combat: SPACE to attack nearby enemy
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            for (size_t i = 0; i < enemies.size(); ++i) {
                float dx = enemies[i].position.x - playerPosition.x;
                float dy = enemies[i].position.y - playerPosition.y;
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance < 24.f) {
                    enemies[i].hp -= playerAttack;
                    if (enemies[i].hp <= 0) {
                        int ex = enemies[i].position.x / TILE_SIZE;
                        int ey = enemies[i].position.y / TILE_SIZE;
                        map[ey][ex] = 0;
                        enemies.erase(enemies.begin() + i);
                    }
                    break;
                }
            }
        }

        // Heal pickup (if player is near heal tile)
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                if (map[y][x] == 6) {
                    float dx = (x * TILE_SIZE + TILE_SIZE / 2.f) - (playerPosition.x + TILE_SIZE / 2.f);
                    float dy = (y * TILE_SIZE + TILE_SIZE / 2.f) - (playerPosition.y + TILE_SIZE / 2.f);
                    float distance = std::sqrt(dx * dx + dy * dy);

                    if (distance < 20.0f && playerHP < playerMaxHP) {
                        playerHP += 30;
                        if (playerHP > playerMaxHP) playerHP = playerMaxHP;
                        map[y][x] = 0;
                    }
                }
            }
        }

        // Update health bar
        float hpPercent = static_cast<float>(playerHP) / playerMaxHP;
        hpText.setString("HP: " + std::to_string(playerHP));
        hpBarFront.setSize(sf::Vector2f(100.f * hpPercent, 10));

        // Drawing the game map
        window.clear();
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                tile.setTexture(map[y][x] == 1 ? &wallTexture : &floorTexture);
                window.draw(tile);

                // Draw heal sprite
                if (map[y][x] == 6) {
                    sf::Sprite healSprite;
                    healSprite.setTexture(healTexture);
                    healSprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                    window.draw(healSprite);
                }
            }
        }

        // Draw all enemies
        for (const auto& enemy : enemies) {
            sf::Sprite enemySprite;
            if (enemy.type == 3) enemySprite.setTexture(goblinTexture);
            if (enemy.type == 4) enemySprite.setTexture(skeletonTexture);
            if (enemy.type == 5) enemySprite.setTexture(demonTexture);
            if (enemy.type == 9) enemySprite.setTexture(bossTexture);
            enemySprite.setPosition(enemy.position);
            window.draw(enemySprite);
        }

        // Draw player and UI
        player.setPosition(playerPosition);
        window.draw(player);
        window.draw(hpText);
        window.draw(hpBarBack);
        window.draw(hpBarFront);
        window.display();
    }

    return 0;
}
