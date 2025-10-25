#include <iostream>
#include <vector>
#include <memory>


class Character {
private:
    std::string name;
    int health;
    int attackPower;
public:
    Character(const std::string& _name, int _health, int _attackPower):
    name(_name),health(_health),attackPower(_attackPower){}

    std::string getName() const {
        return this->name;
    }

    int getHealth() const {
        return this->health;
    }

    int getAttackPower() const {
        return this->attackPower;
    }

    bool isAlive() const {
        return this->health > 0;
    }

    void takeDamage(int damage) {
        if (damage < 0) return;
        if (isAlive() && this->health>=damage)
            this->health-=damage;
    }

    void heal(int amount) {
        if (amount > 0 && this->getHealth()+amount <= 100)
            this->health+=amount;
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
public:
    Player(const std::string& _name, int _health, int _attackPower):
    Character(_name, _health, _attackPower), level(1), experience(0){}

    void gainExperience(int amount) {
        if (amount < 0) return;
        this->experience+=amount;
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
