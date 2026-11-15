#pragma once
#include "damage.hpp"
#include <string>

class PlayerInfo {
public:
    explicit PlayerInfo();

    void getDamage(Damage damage);
    float m_hitPointsMax;
    float m_hitPoints;
    float m_attack;
    float m_defense;

    std::string getHealthText();

private:
};
