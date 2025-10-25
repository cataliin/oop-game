#include <iostream>
#include <vector>
#include <memory>


class Character {
protected:
    std::string name;
    int health;
    int maxHealth;
    int attackPower;
public:
    Character(const std::string& _name, int _health, int _attackPower):
    name(_name),health(_health),attackPower(_attackPower), maxHealth(_health){}

    std::string getName() const {
        return this->name;
    }

    int getHealth() const {
        return this->health;
    }

    int getMaxHealth() const {
        return this->maxHealth;
    }

    int getAttackPower() const {
        return this->attackPower;
    }

    bool isAlive() const {
        return this->health > 0;
    }

    void takeDamage(int damage) {
        if (damage < 0) return;
        this->health-=damage;
        if (this->health<0)
            this->health = 0;
    }

    void heal(int amount) {
        if (amount > 0) {
            this->health+=amount;
            if (this->health > this->maxHealth)
                this->health = this->maxHealth;
        }
    }

    virtual void attack(Character* target) = 0;
};

class Item {
private:
    std::string name;
public:
    Item(const std::string& _name) : name(_name){};

    std::string getName() const {
        return this->name;
    }

    virtual void use(Character* target) = 0;

    virtual ~Item() = default;
};

class Potion : public Item {
public:
    Potion() : Item("healing potion"){};
    void use(Character* target) override {
        std::cout << target->getName() << " used a " << this->getName() << " and healed for 25 HP" << std:: endl;
        // int currentHealth = target->getHealth();
        target->heal(25);
    }
};

class Player: public Character {
private:
    int level;
    int experience;
    std::vector<std::shared_ptr<Item>> inventory;

    void checkLevelUp() {
        while (this->experience >=100) {
            this->level+=1;
            this->experience-=100;

            this->maxHealth+=5;
            this->health = this->maxHealth;
            this->attackPower+=2;

            std::cout << this->getName() << " has just leveled up to " << this->level << "!" << std::endl;
        }
    }

public:
    Player(const std::string& _name, int _health, int _attackPower):
    Character(_name, _health, _attackPower), level(1), experience(0){}

    void gainExperience(int amount) {
        if (amount < 0) return;
        this->experience+=amount;

        this->checkLevelUp();
    }

    void addItem(std::shared_ptr<Item> item) {
        this->inventory.push_back(item);
    }

    void useItem(size_t index) {
        if (index >= this->inventory.size()) {
            std::cout << "Invalid item slot" << std::endl;
            return;
        }
        inventory[index]->use(this);

        inventory.erase(inventory.begin() + index);
    }

    void attack(Character *target) override {
        if (!target->isAlive()) {
            std::cout << "The player " << target->getName() << " is already defeated" << std::endl;
        }
        int damageDealt = this->getAttackPower() + 2*this->level;
        std::cout << this->getName() << " attacks " << target->getName() << " and deals " << damageDealt << "damage" << std::endl;
        target->takeDamage(damageDealt);

        if (!target->isAlive()) {
            std::cout << this->getName() << " has just slain " << target->getName() << std::endl;
            this->gainExperience(15);
        }
    };
};

class Enemy : public Character {
private:
    int experienceDrop;
    int isBoss;
public:
    Enemy(const std::string& _name, int _health, int _attackPower, int _expDrop, int _isBoss = false):
    Character(_name, _health, _attackPower), experienceDrop(_expDrop), isBoss(_isBoss){}

    int getExpDrop() const {
        return this->experienceDrop;
    }

    void attack(Character* target) override {
        if (!target->isAlive()) {
            std::cout << "The player " << target->getName() << " is already defeated" << std::endl;
        }

        int damageDealt = this->getAttackPower();
        std::cout << this->getName() << " has just hit " << target->getName() << " for " << damageDealt << std::endl;

        target->takeDamage(damageDealt);
        if (!target->isAlive()) {
            std::cout << target->getName() << " has been slain" << std::endl;
        }
    }
};


int main() {
    Player p1("Catalin", 100,15);
    std::shared_ptr<Item> potion1 = std::make_shared<Potion>();
    std::shared_ptr<Item> potion2 = std::make_shared<Potion>();

    p1.addItem(potion1);
    p1.addItem(potion2);
    p1.takeDamage(50);
    std::cout << p1.getHealth() << std::endl;
    p1.useItem(0);
    std::cout << p1.getHealth() << std::endl;


    return 0;
}
