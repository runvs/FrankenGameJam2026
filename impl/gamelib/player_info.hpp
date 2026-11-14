#pragma once

class PlayerInfo {
public:
    explicit PlayerInfo();

    void getDamage(float damage);
    float m_hitPointsMax { 100.0f };
    float m_hitPoints { 100.0f };
    float m_attack { 10.0f };
    float m_defense { 10.0f };

private:
};
