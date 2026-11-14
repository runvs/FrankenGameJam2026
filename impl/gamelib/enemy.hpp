#pragma once

#include <animation.hpp>
#include <box2dwrapper/box2d_object.hpp>
#include <encounter/encounter_info.hpp>
#include <game_object.hpp>
#include <particle_system.hpp>
#include <shape.hpp>
#include <memory>

class Enemy : public jt::GameObject {
public:
    explicit Enemy(EnemyEncounterInfo const& enemy_encounter_info);

private:
    // enemy stats
    EnemyEncounterInfo m_enemy_encounter_info;

    // TODO: enemy states

    std::shared_ptr<jt::Animation> m_animation;

    void doCreate() override;
    void doUpdate(float elapsed) override;
    void doDraw() const override;
};

jt::Vector2f enemy_pos_to_vector(int pos_number);
