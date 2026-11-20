#pragma once
#include "damage.hpp"
#include <string>

class PlayerInfo {
public:
    explicit PlayerInfo();

    void getDamage(Damage const& damage);
    float m_hitPointsMax;
    float m_hitPoints;
    float m_attack;
    float m_defense;

    std::string getHealthText();

    // items
    int m_nMedKits { 2 };
    int m_nMolotov { 1 };

private:
};
