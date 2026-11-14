#include "enemy.hpp"
#include <encounter/encounter_info.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <vector.hpp>

Enemy::Enemy(EnemyEncounterInfo const& enemy_encounter_info)
{
    // TODO: implement scaling with level from encounter info
    //  set enemy stats based on encounter info
    m_enemy_encounter_info = enemy_encounter_info;
}

void Enemy::doCreate()
{
    // load sprite / animation
    m_animation = std::make_shared<jt::Animation>();
    m_animation->loadFromAseprite(m_enemy_encounter_info.enemyBase.spritesheet, textureManager());
    m_animation->setPosition(enemy_pos_to_vector(m_enemy_encounter_info.position));
    m_animation->setScale(jt::Vector2f(2.0, 2.0));
    m_animation->play("idle");

    m_healthbar = std::make_shared<jt::Bar>(48.0f, 8.0f, true, textureManager());
    m_healthbar->setBackColor(jt::colors::Gray);
    m_healthbar->setColor(jt::colors::Red);
    m_healthbar->setMaxValue(m_enemy_encounter_info.enemyBase.hitPointsMax);
    m_healthbar->setCurrentValue(m_enemy_encounter_info.enemyBase.hitPoints);
    jt::Vector2f const offset { 0.0f, -16.0f };
    m_healthbar->setPosition(enemy_pos_to_vector(m_enemy_encounter_info.position) + offset);
}

void Enemy::doUpdate(float const elapsed)
{
    m_animation->update(elapsed);
    m_healthbar->setMaxValue(m_enemy_encounter_info.enemyBase.hitPointsMax);
    m_healthbar->setCurrentValue(m_enemy_encounter_info.enemyBase.hitPoints);
    jt::Vector2f const offset { 0.0f, -16.0f };
    m_healthbar->setPosition(enemy_pos_to_vector(m_enemy_encounter_info.position) + offset);
    m_healthbar->update(elapsed);
}

void Enemy::doDraw() const
{
    m_animation->draw(renderTarget());
    m_healthbar->draw(renderTarget());
}

jt::Vector2f enemy_pos_to_vector(int const pos_number)
{
    switch (pos_number) {
    case 0:
        return jt::Vector2f(145.0f, 170.0f);
    case 1:
        return jt::Vector2f(195.0f, 150.0f);
    case 2:
        return jt::Vector2f(235.0f, 170.0f);
    case 3:
        return jt::Vector2f(275.0f, 150.0f);
    default:
        return jt::Vector2f(0.0f, 0.0f);
    }
}
