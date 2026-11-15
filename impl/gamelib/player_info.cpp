#include "player_info.hpp"

#include "damage.hpp"
#include <cmath>

PlayerInfo::PlayerInfo() { };

void PlayerInfo::getDamage(Damage damage)
{
    // TODO take count into effect
    m_hitPoints -= damage.value;
}

std::string PlayerInfo::getHealthText()
{
    auto hitPointString
        = std::to_string(static_cast<int>(std::round(m_hitPoints)));
    auto hitPointsMaxString
        = std::to_string(static_cast<int>(std::round(m_hitPointsMax)));
    auto healthString = hitPointString + " / " + hitPointsMaxString;
    return healthString;
}
