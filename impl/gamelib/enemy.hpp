#pragma once

#include "bar.hpp"
#include "state_encounter.hpp"
#include <animation.hpp>
#include <box2dwrapper/box2d_object.hpp>
#include <encounter/encounter_info.hpp>
#include <game_object.hpp>
#include <particle_system.hpp>
#include <shape.hpp>
#include <text.hpp>
#include <memory>

class Enemy : public jt::GameObject {
public:
    explicit Enemy(EnemyEncounterInfo const& enemy_encounter_info);

    void getDamage(float damage);
    float doAttack();

    void doAction(StateEncounter& state_encounter);

private:
    // enemy stats
    EnemyEncounterInfo m_enemy_encounter_info;

    // TODO: enemy states

    std::shared_ptr<jt::Animation> m_animation;
    std::shared_ptr<jt::Bar> m_healthbar;
    std::shared_ptr<jt::Text> m_healthtext;

    void doCreate() override;
    void doUpdate(float elapsed) override;
    void doDraw() const override;

    jt::Vector2f const m_healthbar_offset { 0.0f, -16.0f };
    jt::Vector2f const m_healthtext_offset { 25.0f, -19.0f };

    std::string get_health_text();
};

jt::Vector2f enemy_pos_to_vector(int pos_number);
