#include "level_encounter.hpp"

#include "game_properties.hpp"

#include <filesystem>

LevelEncounter::LevelEncounter(EncounterInfo const& info, jt::Vector2f const& pos)
    : m_info { info }
    , m_pos { pos }
{
}

void LevelEncounter::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();
    m_animation->loadFromAseprite(
        (std::filesystem::path { "assets" } / m_info.levelSprite).string(), textureManager());
    m_animation->setPosition(m_pos);
    
    m_animation->play("idle");
    m_animation->setZ(GP::LevelZLayerPlayer());
}

void LevelEncounter::doUpdate(float const elapsed) { m_animation->update(elapsed); }

void LevelEncounter::doDraw() const { m_animation->draw(renderTarget()); }
