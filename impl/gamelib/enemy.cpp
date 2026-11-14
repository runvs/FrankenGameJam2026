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
}

void Enemy::doUpdate(float const elapsed) { m_animation->update(elapsed); }

void Enemy::doDraw() const { m_animation->draw(renderTarget()); }

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
