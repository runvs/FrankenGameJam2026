#pragma once
#include "animation.hpp"
#include "encounter_info.hpp"
#include <game_object.hpp>

class LevelEncounter : public jt::GameObject {
public:
    LevelEncounter(EncounterInfo const& info, jt::Vector2f const& pos);

    jt::Vector2f getPosition() const;

private:
    void doCreate() override;
    void doUpdate(float elapsed) override;
    void doDraw() const override;

    EncounterInfo m_info;
    jt::Vector2f m_pos;
    std::shared_ptr<jt::Animation> m_animation;
};
