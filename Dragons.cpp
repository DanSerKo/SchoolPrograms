#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <Windows.h>

std::mt19937 rnd(time(0));
HANDLE hConsole;

class Attacker {
public:
    Attacker() {
        _health = rnd() % 5 + 1;
        _attack = rnd() % 5 + 1;
    }
    Attacker(int health, int attack) {
        _health = rnd() % health + 1;
        _attack = rnd() % attack + 1;
    }
    void attack(Attacker& target) {
        target._health -= _attack;
    }
    bool is_alive() {
        return _health > 0;
    }
private:
    int _health, _attack;
};

class Hero : public Attacker {
public:
    Hero(): Attacker() {
        _name = "Hero";
        _experience = 0;
    }
    Hero(int health, int attack): Attacker(health, attack) {
        _name = "Hero";
        _experience = 0;
    }
    ~Hero() {
        system("color 0F");
        if (!is_alive()) {
            SetConsoleTextAttribute(hConsole, 4);
            std::cout << "Hero died :(\n";
        }
        else {
            SetConsoleTextAttribute(hConsole, 13);
            std::cout << "Hero win!!!\n";
        }
        std::cout << "Experience: " << _experience;
    }
    void _init_(std::string name) {
        _name = name;
        std::cout << _name << " went for a walk\n";
    }
    std::string get_name() {
        return _name;
    }
    void add_exp() {
        _experience++;
    }
private:
    int _experience;
    std::string _name;
};

class Enemy : public Attacker {
public:
    Enemy() : Attacker() {}
    Enemy(int health, int attack): Attacker(health, attack) {}
};

class Dragon : public Enemy {
public:
    Dragon(): Enemy() {
        _color = "White";
    }
    Dragon(int health, int attack): Enemy(health, attack) {
        _color = "White";
    }
    void check() {
        if (!is_alive()) {
            SetConsoleTextAttribute(hConsole, 13);
            std::cout << "You kill " << _color << " Dragon\n";
        }
    }
    std::string get_color() {
        return _color;
    }
    bool check_answer(int heros_answer) {
        return _answer == heros_answer;
    }
    void make_quest(int level) {
        int mod = (1 << (3 * level));
        _quest = { rnd() % mod, rnd() % mod };
    }
    virtual void quest(int level) {
        make_quest(level);
    }
    int get_answer() {
        return _answer;
    }
    std::pair <int, int> get_quest() {
        return _quest;
    }
protected:
    std::string _color;
    void set_answer(int answer) {
        _answer = answer;
    }
private:
    long long _answer;
    std::pair <int, int> _quest;
};

class RedDragon : public Dragon {
public:
    RedDragon(): Dragon() {
        system("color 40");
        _color = "Red";
    }
    RedDragon(int health, int attack): Dragon(health, attack) {
        system("color 40");
        _color = "Red";
    }
    void quest(int level) override {
        make_quest(level);
        auto q = get_quest();
        set_answer(q.first - q.second);
    }
private:
};

class GreenDragon : public Dragon {
public:
    GreenDragon(): Dragon() {
        system("color 20");
        _color = "Green";
    }
    GreenDragon(int health, int attack) : Dragon(health, attack) {
        system("color 20");
        _color = "Green";
    }
    void quest(int level) override {
        make_quest(level);
        auto q = get_quest();
        set_answer(q.first + q.second);
    }
private:
};

class BlackDragon : public Dragon {
public:
    BlackDragon() : Dragon() {
        system("color 0F");
        _color = "Black";
    }
    BlackDragon(int health, int attack) : Dragon(health, attack) {
        system("color 0F");
        _color = "Black";
    }
    void quest(int level) override {
        make_quest(level);
        auto q = get_quest();
        set_answer(q.first * q.second);
    }
private:
};

void Cin(int& a) {
    a = -(1 << 31);
    std::string s;
    std::getline(std::cin, s);
    if (!s.size() || s.size() > 8)
        return;
    for (auto& r : s)
        if ((r < '0' || r > '9') && r != '-')
            return;
    a = stoi(s);
}

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 9);
    Hero hero(15, 3);
    std::cout << "Hello, enter the name of the hero:\n";
    std:: string s;
    std::getline(std::cin, s);
    hero._init_(s);
    int level = -1, cnt, max_hp, max_at;
    while (level != 1 && level != 2 && level != 3) {
        std::cout << "Choose the difficulty level (1/2/3):\n";
        Cin(level);
        cnt = level * 5;
        max_hp = level * 5;
        max_at = level * 2;
    }
    
    for (int i = 0; i < cnt && hero.is_alive(); i++) {
        Dragon* D = nullptr;
        auto rng = rnd();
        if (rng % 3 == 0)
            D = new RedDragon(max_hp, max_at);
        if (rng % 3 == 1)
            D = new GreenDragon(max_hp, max_at);
        if (rng % 3 == 2)
            D = new BlackDragon(max_hp, max_at);

        SetConsoleTextAttribute(hConsole, 12);
        std::cout << "New " << D->get_color() << " Dragon!!!\n";
        
        while (D->is_alive() && hero.is_alive()) {
            D->quest(level);
            SetConsoleTextAttribute(hConsole, 11);
            std::cout << D->get_color() << " Dragon asks: " << D->get_quest().first << ' ' << D->get_quest().second << "\n";
            
            int answer;
            Cin(answer);
            if (D->check_answer(answer)) {
                SetConsoleTextAttribute(hConsole, 10);
                std::cout << "YES\n";
                hero.add_exp();
                hero.attack(*D);
            }
            else {
                SetConsoleTextAttribute(hConsole, 12);
                std::cout << "NO " << D->get_answer() << '\n';
                D->attack(hero);
            } 
        }
        D->check();
    }
}
