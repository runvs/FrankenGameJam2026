#pragma once
#include "damage.hpp"
#include <string>

class PlayerInfo {
public:
    explicit PlayerInfo();

    void getDamage(Damage damage);
    float m_hitPointsMax { 100.0f };
    float m_hitPoints { 100.0f };
    float m_attack { 10.0f };
    float m_defense { 10.0f };

    std::string getHealthText();

private:
};
