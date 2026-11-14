#include "enemy.hpp"
#include <encounter_info.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>
#include <vector.hpp>

Enemy::Enemy(EnemyEncounterInfo enemy_encounter_info)
{
    // TODO: implement scaling with level from encounter info
    //  set enemy stats based on encounter info
    m_hit_points = enemy_encounter_info.enemyBase.hitPoints;
    m_max_hit_points = enemy_encounter_info.enemyBase.hitPointsMax;
    m_attack = enemy_encounter_info.enemyBase.Attack;
    m_defense = enemy_encounter_info.enemyBase.Defense;

    // load sprite / animation
    m_animation = std::make_shared<jt::Animation>();
    m_animation->loadFromAsperite(
        (std::filesystem::path { "assets" }
            / enemy_encounter_info.enemyBase.spritesheet)
            .sting(),
        textureManager());
    m_animation->setPosition(
        enemy_pos_to_vector(enemy_encounter_info.position));
    m_animation.setScale(jt::Vector2f(2.0, 2.0));
}

void Enemy::doCreate() { m_animation->play("idle"); }

void Enemy::doUpdate(float elapsed) { }

void Enemy::doDraw() const { m_animation->draw(renderTarget()); }

void Enemy::updateAnimation(float elapsed) { m_animation->update(elapsed); }

jt::Vector2f enemy_pos_to_vector(int pos_number)
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
