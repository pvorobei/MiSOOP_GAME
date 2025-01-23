#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>

// Базовый класс для всех персонажей
class Character {
protected:
    std::string name;
    int health;
    int attackPower;

public:
    Character(std::string n, int h, int ap) : name(n), health(h), attackPower(ap) {}

    virtual void attack(Character &target) {
        std::cout << name << " атакует " << target.getName() << " и сносит ему " << attackPower << " ХП!" << std::endl;
        target.takeDamage(attackPower);
    }

    virtual void takeDamage(int damage) {
        health -= damage;
        if (health <= 0) {
            health = 0;
            std::cout << name << " был повержен!" << std::endl;
        }
    }

    bool isAlive() const {
        return health > 0;
    }

    std::string getName() const {
        return name;
    }

    virtual ~Character() {}
};

// Класс игорька
class Player : public Character {
public:
    Player(std::string n) : Character(n, 100, 20) {}

    void attack(Character &target) override {
        std::cout << name << " (игорёк) атакует " << target.getName() << " и сносит ему " << attackPower << " ХП!" << std::endl;
        target.takeDamage(attackPower);
    }
};

// Класс врага
class Enemy : public Character {
public:
    Enemy(std::string n, int h, int ap) : Character(n, h, ap) {}

    void attack(Character &target) override {
        std::cout << name << " (враг) атакует " << target.getName() << " и сносит ему " << attackPower << " ХП!" << std::endl;
        target.takeDamage(attackPower);
    }
};

// Базовый класс для предметов
class Item {
protected:
    std::string itemName;

public:
    Item(std::string name) : itemName(name) {}

    virtual void use(Player &player) = 0;

    std::string getName() const {
        return itemName;
    }

    virtual ~Item() {}
};

// Класс исцеляющего предмета
class HealingPotion : public Item {
private:
    int healingAmount;

public:
    HealingPotion(std::string name, int amount) : Item(name), healingAmount(amount) {}

    void use(Player &player) override {
        std::cout << player.getName() << " использует " << itemName 
                  << ", восстанавливая " << healingAmount << " ХП!" << std::endl;
    }
};

// Класс для управления инвентарем
class Inventory {
private:
    std::vector<Item*> items;

public:
    void addItem(Item* item) {
        items.push_back(item);
    }

    void useItem(Player &player, const std::string &itemName) {
        for (auto item : items) {
            if (item->getName() == itemName) {
                item->use(player);
                return; 
            }
        }
        std::cout << player.getName() << ", у тебя нет такого предмета!" << std::endl;
    }

    ~Inventory() {
        for (auto item : items) {
            delete item;
        }
    }
};

// Класс игры
class Game {
private:
    Player player;
    std::vector<Enemy> enemies;
    Inventory inventory;

public:
    Game(std::string playerName) : player(playerName) {
        // Создаем врагов
        enemies.push_back(Enemy("🤬", 30, 10));
        enemies.push_back(Enemy("😤", 50, 15));
        enemies.push_back(Enemy("🥶", 100, 25));

        // Создаем предметы и добавляем их в инвентарь
        inventory.addItem(new HealingPotion("Малое восстановление", 20));
        inventory.addItem(new HealingPotion("Большое восстановление", 50));
    }

    void start() {
        for (auto &enemy : enemies) {
            while (player.isAlive() && enemy.isAlive()) {
                player.attack(enemy);
                if (enemy.isAlive()) {
                    enemy.attack(player);
                }
            }
            if (!player.isAlive())
                break;
            std::cout << "\n";
        }

        if (player.isAlive()) {
            std::cout << player.getName() << ", ты победил!" << std::endl;
        } else {
            std::cout << player.getName() << ", ты проиграл!" << std::endl;
        }
        // Инвентарь освобождается автоматически при уничтожении объекта Game
    }

    ~Game() {}
};

// Класс для управления состоянием игры
class GameStateManager {
private:
    Game* currentGame;

public:
    void startNewGame(const std::string& playerName) {
        currentGame = new Game(playerName);
        currentGame->start();
        delete currentGame; // Освобождаем память после завершения игры
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);

    GameStateManager gameStateManager;
    
    gameStateManager.startNewGame("😎");
    
    return 0;
}
