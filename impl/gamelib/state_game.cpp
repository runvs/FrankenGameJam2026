#include "state_game.hpp"
#include <box2dwrapper/box2d_world_impl.hpp>
#include <color/color.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <hud/hud.hpp>
#include <screeneffects/vignette.hpp>
#include <shape.hpp>
#include <state_menu.hpp>

void StateGame::onCreate()
{
    m_world = std::make_shared<jt::Box2DWorldImpl>(jt::Vector2f { 0.0f, 0.0f });

    for (auto const layerID : GP::LevelZLayerAllLayers()) {
        getGame()->gfx().createZLayer(layerID);
    }

    float const w = static_cast<float>(GP::GetWindowSize().x);
    float const h = static_cast<float>(GP::GetWindowSize().y);

    using jt::Shape;

    m_background = std::make_shared<Shape>();
    m_background->makeRect({ w, h }, textureManager());
    m_background->setColor(GP::PaletteBackground());
    m_background->setIgnoreCamMovement(true);
    m_background->update(0.0f);
    m_background->setZ(GP::LevelZLayerBelowPlayer());

    loadLevel();

    createPlayer();

    m_vignette = std::make_shared<jt::Vignette>(GP::GetScreenSize());
    add(m_vignette);
    m_vignette->setZ(GP::LevelZLayerAboveAll());

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    // StateGame will call drawObjects itself.
    setAutoDraw(false);
}

void StateGame::onEnter() { }

void StateGame::createPlayer()
{
    m_player = std::make_shared<Player>(m_world);
    add(m_player);
    m_player->setPosition(m_level->getPlayerStartingPos());
}

void StateGame::cleanUpLoadedLevel()
{
    if (!m_levelEncounters) {
        return;
    }
    for (auto const& e : *m_levelEncounters) {
        auto const encounter = e.lock();
        encounter->kill();
    }
    m_level->getColliders().clear();
}

void StateGame::loadLevel(std::string const& levelName)
{
    // clean up old level
    cleanUpLoadedLevel();

    jt::tilemap::TilesonLoader loader { getGame()->cache().getTilemapCache(), levelName };

    m_level = std::make_shared<Level>(m_world);

    add(m_level);
    m_level->loadTileLayers(loader);
    m_level->loadLevelCollisions(loader);

    m_levelEncounters = std::make_shared<jt::ObjectGroup<LevelEncounter>>();

    for (auto const& encounterInfoRect : m_level->getEncounters()) {
        auto const encounterJson = std::filesystem::path { "assets" }
            / encounterInfoRect.properties.strings.at("encounter_info");
        std::ifstream f { encounterJson };
        nlohmann::json const j = nlohmann::json::parse(f);
        EncounterInfo encounterInfo = j;

        auto levelEncounter
            = std::make_shared<LevelEncounter>(encounterInfo, encounterInfoRect.position);
        m_levelEncounters->push_back(levelEncounter);
        add(levelEncounter);
    }
}

void StateGame::updateLevelEncounters(float const elapsed)
{
    m_levelEncounters->update(elapsed);
    for (auto const& e : *m_levelEncounters) {
        auto encounter = e.lock();
        auto const pos = encounter->getPosition();
        auto const distance = jt::MathHelper::distanceBetween(
            pos, m_player->getPosition() - GP::LevelTileSize() * 0.5f);
        if (distance <= jt::MathHelper::length(GP::LevelTileSize()) * 0.75f) {
            std::cout << "trigger encounter\n";
            // TODO trigger encounter
        }
    }
}

void StateGame::onUpdate(float const elapsed)
{
    if (m_running) {
        m_world->step(elapsed, GP::PhysicSubSteps());
        if (getGame()->input().keyboard()->pressed(jt::KeyCode::LShift)
            && getGame()->input().keyboard()->pressed(jt::KeyCode::Escape)) {
            endGame();
        }
        if (getGame()->input().keyboard()->justPressed(jt::KeyCode::F8)) {
            loadLevel("assets/test_level.json");
            m_player->setPosition(m_level->getPlayerStartingPos());
        }
        if (getGame()->input().keyboard()->justPressed(jt::KeyCode::F9)) {
            loadLevel("assets/test_level2.json");
            m_player->setPosition(m_level->getPlayerStartingPos());
        }

        // TODO scroll/camera mechanics

        // TODO change level mechanic

        updateLevelEncounters(elapsed);
    }

    m_background->update(elapsed);
    m_vignette->update(elapsed);
}

void StateGame::onDraw() const
{
    m_background->draw(renderTarget());
    drawObjects();

    m_vignette->draw();
    m_hud->draw();
}

void StateGame::endGame()
{
    if (m_hasEnded) {
        // trigger this function only once
        return;
    }
    m_hasEnded = true;
    m_running = false;

    getGame()->stateManager().switchToStoredState("menu");
}

std::string StateGame::getName() const { return "State Game"; }
