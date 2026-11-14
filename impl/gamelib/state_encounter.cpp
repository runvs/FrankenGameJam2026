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
StateEncounter::StateEncounter(EncounterInfo encounter_info) { m_encounter_info = encounter_info; }

// destructor
// StateEncounter::~StateEncounter() { }

void StateEncounter::onCreate()
{
    m_world = std::make_shared<jt::Box2DWorldImpl>(jt::Vector2f { 0.0f, 0.0f });

    for (auto const layerID : GP::LevelZLayerAllLayers()) {
        getGame()->gfx().createZLayer(layerID);
    }

    m_background = std::make_shared<jt::Sprite>(
        (std::filesystem::path { "assets" } / m_encounter_info.backgroundSprite).string(),
        textureManager());

    createPlayer();

    createEnemies();

    m_vignette = std::make_shared<jt::Vignette>(GP::GetScreenSize());
    add(m_vignette);
    m_vignette->setZ(GP::LevelZLayerAboveAll());

    m_battleMenu = std::make_shared<BattleMenu>();
    add(m_battleMenu);
    m_battleMenu->m_addTween = [this](std::shared_ptr<jt::TweenInterface> tw) { add(tw); };

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    // StateGame will call drawObjects itself.
    setAutoDraw(false);
}

void StateEncounter::onEnter() { };

void StateEncounter::createEnemies()
{
    m_enemies = std::make_shared<jt::ObjectGroup<Enemy>>();
    for (auto const enemy_encounter_info : m_encounter_info.enemies) {
        auto enemy = std::make_shared<Enemy>(enemy_encounter_info);
        add(enemy);
        m_enemies->push_back(enemy);
    }
}

void draw_enemy(EnemyEncounterInfo enemy_encounter_info) { }

void StateEncounter::createPlayer()
{
    m_player = std::make_shared<jt::Animation>();
    m_player->loadFromAseprite("assets/chars.aseprite", textureManager());
    m_player->play("idle");
    // TODO adjust player position
    m_player->setPosition(jt::Vector2f(70.0f, 160.0f));
    m_player->setScale(jt::Vector2f(-2.0f, 2.0f));
}

std::string StateEncounter::getName() const { return "State Encounter"; }

void StateEncounter::onUpdate(float const elapsed)
{
    m_background->update(elapsed);
    m_player->update(elapsed);
    m_vignette->update(elapsed);
    m_enemies->update(elapsed);
}

void StateEncounter::onDraw() const
{
    m_background->draw(renderTarget());
    m_player->draw(renderTarget());

    for (auto const& e : *m_enemies) {
        auto const enemy = e.lock();
        enemy->draw();
    }

    m_vignette->draw();

    m_battleMenu->draw();
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
