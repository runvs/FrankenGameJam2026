#include "state_encounter.hpp"
#include <box2dwrapper/box2d_world_impl.hpp>
#include <color/color.hpp>
#include <encounter/encounter_info.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <hud/hud.hpp>
#include <screeneffects/vignette.hpp>
#include <shape.hpp>
#include <sprite.hpp>
#include <state_menu.hpp>

// constructor
StateEncounter::StateEncounter(EncounterInfo encounter_info)
{
    m_encounter_info = encounter_info;
}

// destructor
// StateEncounter::~StateEncounter() { }

void StateEncounter::onCreate()
{
    m_world = std::make_shared<jt::Box2DWorldImpl>(jt::Vector2f { 0.0f, 0.0f });

    for (auto const layerID : GP::LevelZLayerAllLayers()) {
        getGame()->gfx().createZLayer(layerID);
    }

    float const w = static_cast<float>(GP::GetWindowSize().x);
    float const h = static_cast<float>(GP::GetWindowSize().y);

    using jt::Shape;

    m_background = std::make_shared<jt::Sprite>(
        m_encounter_info.backgroundSprite, textureManager());

    // loadLevel();

    createPlayer();

    m_vignette = std::make_shared<jt::Vignette>(GP::GetScreenSize());
    add(m_vignette);
    m_vignette->setZ(GP::LevelZLayerAboveAll());

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    // StateGame will call drawObjects itself.
    setAutoDraw(false);
}

void EncounterState::onEnter() { };

void EncounterState::createPlayer()
{
    m_player = std::make_shared<Player>(m_world);
    add(m_player);
    // TODO adjust player position
    m_player->setPosition(jt::Vector2f(256, 720));
}

std::string StateEncounter::getName() const { return "State Encounter"; }

void StateEncounter::onDraw() const
{
    m_background->draw(renderTarget());
    drawObjects();

    m_vignette->draw();
    m_hud->draw();
}

void StateEncounter::endGame()
{
    if (m_hasEnded) {
        // trigger this function only once
        return;
    }
    m_hasEnded = true;
    m_running = false;

    getGame()->stateManager().switchToStoredState("menu");
}

void StateEncounter::onUpdate(float const elapsed)
{
    m_background->update(elapsed);
    m_vignette->update(elapsed);
}
