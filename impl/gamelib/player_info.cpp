#include "player_info.hpp"

#include "damage.hpp"

PlayerInfo::PlayerInfo() { };

void PlayerInfo::getDamage(Damage damage)
{
    // TODO take count into effect
    m_hitPoints -= damage.value;
}
