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
    } else if (m_type == PickupType::Armor1) {
        m_sprite = std::make_shared<jt::Sprite>("assets/armor1.png", textureManager());
    } else if (m_type == PickupType::Armor2) {
        m_sprite = std::make_shared<jt::Sprite>("assets/armor2.png", textureManager());
    } else if (m_type == PickupType::Armor3) {
        m_sprite = std::make_shared<jt::Sprite>("assets/armor3.png", textureManager());
    }
    m_sprite->setPosition(m_pos);
    m_sprite->setZ(GP::LevelZLayerPlayer());
    m_sprite->setOffset(GP::LevelTileSize() * 0.125f);
}

void LevelPickup::doUpdate(float const elapsed) { m_sprite->update(elapsed); }

void LevelPickup::doDraw() const { m_sprite->draw(renderTarget()); }

jt::Vector2f LevelPickup::getPosition() const { return m_pos; }

PickupType LevelPickup::getPickupType() const { return m_type; }
