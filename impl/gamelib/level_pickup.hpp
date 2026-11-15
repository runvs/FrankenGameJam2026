#pragma once
#include "animation.hpp"
#include <game_object.hpp>

#include <cstdint>

enum class PickupType : std::uint8_t { Medkit, Molotov, Armor1, Armor2, Armor3 };

class LevelPickup : public jt::GameObject {
public:
    LevelPickup(PickupType const& info, jt::Vector2f const& pos);

    jt::Vector2f getPosition() const;
    PickupType getPickupType() const;

    // EncounterInfo m_info;

private:
    void doCreate() override;
    void doUpdate(float elapsed) override;
    void doDraw() const override;

    PickupType m_type;
    jt::Vector2f m_pos;
    std::shared_ptr<jt::Sprite> m_sprite;
};
