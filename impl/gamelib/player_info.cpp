#include "player_info.hpp"
#include "damage.hpp"
#include <game_properties.hpp>
#include <cmath>

PlayerInfo::PlayerInfo()
{
    m_hitPointsMax = GP::PlayerInitialHitPointsMax();
    m_hitPoints = GP::PlayerInitialHitPoints();
    m_attack = GP::PlayerInitialAttack();
    m_defense = GP::PlayerInitialDefense();
};

void PlayerInfo::getDamage(Damage damage) { m_hitPoints -= damage.value * (1.0f - m_defense); }

std::string PlayerInfo::getHealthText()
{
    auto hitPointString = std::to_string(static_cast<int>(std::round(m_hitPoints)));
    auto hitPointsMaxString = std::to_string(static_cast<int>(std::round(m_hitPointsMax)));
    auto healthString = hitPointString + " / " + hitPointsMaxString;
    return healthString;
}
