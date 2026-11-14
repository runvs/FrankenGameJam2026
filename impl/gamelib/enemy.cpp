#include "enemy.hpp"
#include "drawable_helpers.hpp"
#include "state_game.hpp"
#include <encounter/encounter_info.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <vector.hpp>
#include <cmath>

Enemy::Enemy(EnemyEncounterInfo const& enemy_encounter_info)
{
    // TODO: implement scaling with level from encounter info
    //  set enemy stats based on encounter info
    m_enemy_encounter_info = enemy_encounter_info;
}

void Enemy::getDamage(float damage)
{
    // TODO anim
    m_enemy_encounter_info.enemyBase.hitPoints -= damage;
    m_animation->flash(0.25f, jt::colors::Red);

    // TODO damage resistance
    if (m_enemy_encounter_info.enemyBase.hitPoints <= 0) {
        kill();
    }
}

void Enemy::doAction(StateEncounter& state_encounter)
{
    m_animation->flash(0.25f, jt::colors::Green);

    state_encounter.damagePlayer(2.0f);

    // std::cout << "!!!!!trigger next actor from enemy doAction\n";
    state_encounter.nextActor();
}

void Enemy::doCreate()
{
    // load sprite / animation
    m_animation = std::make_shared<jt::Animation>();
    m_animation->loadFromAseprite(m_enemy_encounter_info.enemyBase.spritesheet, textureManager());
    m_animation->setPosition(enemy_pos_to_vector(m_enemy_encounter_info.position));
    m_animation->setScale(jt::Vector2f(2.0, 2.0));
    m_animation->play("idle");

    // load healthbar
    m_healthbar = std::make_shared<jt::Bar>(48.0f, 8.0f, true, textureManager());
    m_healthbar->setBackColor(jt::colors::Gray);
    m_healthbar->setColor(jt::colors::Red);
    m_healthbar->setMaxValue(m_enemy_encounter_info.enemyBase.hitPointsMax);
    m_healthbar->setCurrentValue(m_enemy_encounter_info.enemyBase.hitPoints);
    m_healthbar->setPosition(
        enemy_pos_to_vector(m_enemy_encounter_info.position) + m_healthbar_offset);

    // load healthtext
    m_healthtext = jt::dh::createText(renderTarget(), get_health_text(), 10u);
    m_healthtext->setPosition(
        enemy_pos_to_vector(m_enemy_encounter_info.position) + m_healthtext_offset);
}

void Enemy::doUpdate(float const elapsed)
{
    m_animation->update(elapsed);

    // update healthbar
    m_healthbar->setMaxValue(m_enemy_encounter_info.enemyBase.hitPointsMax);
    m_healthbar->setCurrentValue(m_enemy_encounter_info.enemyBase.hitPoints);
    m_healthbar->setPosition(
        enemy_pos_to_vector(m_enemy_encounter_info.position) + m_healthbar_offset);
    m_healthbar->update(elapsed);

    // update healthtext
    m_healthtext = jt::dh::createText(renderTarget(), get_health_text(), 10u);
    m_healthtext->setPosition(
        enemy_pos_to_vector(m_enemy_encounter_info.position) + m_healthtext_offset);
    m_healthtext->setTextAlign(jt::Text::TextAlign::CENTER);
    m_healthtext->update(elapsed);
}

void Enemy::doDraw() const
{
    m_animation->draw(renderTarget());
    m_healthbar->draw(renderTarget());
    m_healthtext->draw(renderTarget());
}

std::string Enemy::get_health_text()
{
    std::string hp_string
        = std::to_string(static_cast<int>(std::round(m_enemy_encounter_info.enemyBase.hitPoints)));

    std::string hp_max_string = std::to_string(
        static_cast<int>(std::round(m_enemy_encounter_info.enemyBase.hitPointsMax)));

    std::string health_text = hp_string + " / " + hp_max_string;

    return health_text;
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
