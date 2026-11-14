#include "player_info.hpp"

PlayerInfo::PlayerInfo() { };

void PlayerInfo::getDamage(float damage) { m_hitPoints -= damage; }
