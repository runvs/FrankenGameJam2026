#include "level_pickup.hpp"

#include "game_properties.hpp"
#include "sprite.hpp"

#include <filesystem>

LevelPickup::LevelPickup(PickupType const& info, jt::Vector2f const& pos)
    : m_type { info }
    , m_pos { pos }
{
}

void LevelPickup::doCreate()
{
    if (m_type == PickupType::Medkit) {
        m_sprite = std::make_shared<jt::Sprite>("assets/medkit.png", textureManager());
    } else if (m_type == PickupType::Molotov) {
        m_sprite = std::make_shared<jt::Sprite>("assets/molotov.png", textureManager());
    }
    m_sprite->setPosition(m_pos);
    m_sprite->setZ(GP::LevelZLayerPlayer());
}

void LevelPickup::doUpdate(float const elapsed) { m_sprite->update(elapsed); }

void LevelPickup::doDraw() const { m_sprite->draw(renderTarget()); }

jt::Vector2f LevelPickup::getPosition() const { return m_pos; }

PickupType LevelPickup::getPickupType() const { return m_type; }
