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
    explicit Enemy(EnemyEncounterInfo enemy_encounter_info);

    ~Enemy() override = default;

    // std::shared_ptr<jt::Animation> getAnimation();

    // jt::Vector2f getPosOnScreen() const;
    // void setPosition(jt::Vector2f const& pos);
    // jt::Vector2f getPosition() const;

private:
    // enemy stats
    float m_hit_points { 50.0f };
    float m_max_hit_points { 100.0f };
    float m_attack { 10.0f };
    float m_defense { 10.0f };

    // TODO: enemy states

    std::shared_ptr<jt::Animation> m_animation;

    void doCreate() override;
    void doUpdate(float elapsed) override;
    void doDraw() const override;

    void updateAnimation(float elapsed);
};

jt::Vector2f enemy_pos_to_vector(int pos_number);
