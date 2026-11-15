#include "state_game.hpp"

#include "camera_helpers.hpp"
#include "drawable_helpers.hpp"
#include "state_encounter.hpp"
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

    m_stars = std::make_shared<jt::Stars>(
        60, jt::Color { 255, 255, 200 }, GP::GetScreenSize());
    add(m_stars);
    m_stars->setZ(GP::LevelZLayerBelowPlayer());
    m_stars->setCamMovementFactor(0.5f);

    loadLevel();

    createPlayer();
    m_playerInfo = PlayerInfo();

    m_vignette = std::make_shared<jt::Vignette>(GP::GetScreenSize());
    add(m_vignette);
    m_vignette->setZ(GP::LevelZLayerAboveAll());

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    // add visualization of medkits and molotov cocktails
    m_medkitSprite
        = std::make_shared<jt::Sprite>("assets/medkit.png", textureManager());
    m_medkitSprite->setIgnoreCamMovement(true);
    m_medkitSprite->setZ(GP::LevelZLayerAboveAll());
    m_medkitSprite->setPosition(m_medkitPosition);

    m_medkitText = jt::dh::createText(
        renderTarget(), std::to_string(m_playerInfo.m_nMedKits), 18u);
    m_medkitText->setPosition(m_medkitPosition + jt::Vector2f(20.0f, 0));
    m_medkitText->setIgnoreCamMovement(true);
    m_medkitText->setZ(GP::LevelZLayerAboveAll());

    m_molotovSprite
        = std::make_shared<jt::Sprite>("assets/molotov.png", textureManager());
    m_molotovSprite->setIgnoreCamMovement(true);
    m_molotovSprite->setZ(GP::LevelZLayerAboveAll());
    m_molotovSprite->setPosition(m_molotovPosition);

    m_molotovText = jt::dh::createText(
        renderTarget(), std::to_string(m_playerInfo.m_nMolotov), 18u);
    m_molotovText->setPosition(m_molotovPosition + jt::Vector2f(20.0f, 0));
    m_molotovText->setIgnoreCamMovement(true);
    m_molotovText->setZ(GP::LevelZLayerAboveAll());
    // StateGame will call drawObjects itself.
    setAutoDraw(false);
}

void StateGame::onEnter() { m_running = true; }

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
    m_level->kill();
}

void StateGame::loadLevel(std::string const& levelName)
{
    // clean up old level
    cleanUpLoadedLevel();

    jt::tilemap::TilesonLoader loader { getGame()->cache().getTilemapCache(),
        levelName };

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

        auto levelEncounter = std::make_shared<LevelEncounter>(
            encounterInfo, encounterInfoRect.position);
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
            m_running = false;
            encounter->kill();
            getGame()->stateManager().storeCurrentState("beforeEncounter");
            auto encounter_state = std::make_shared<StateEncounter>(
                encounter->getEncounterInfo());
            encounter_state->m_stateGame = this;
            getGame()->stateManager().switchState(encounter_state);
        }
    }
}

void StateGame::handleDebugInput()
{
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
}

void StateGame::onUpdate(float const elapsed)
{
    if (m_running) {
        m_world->step(elapsed, GP::PhysicSubSteps());
        handleDebugInput();

        float const margin
            = GP::CameraMarginTileScaleValue() * GP::LevelTileSize().x;
        jt::CameraHelpers::followWithMargin(getGame()->gfx().camera(),
            m_player->getPosition(), GP::GetScreenSize(), elapsed, margin,
            GP::CameraSpeed());

        // TODO change level mechanic
        for (auto const& o : m_level->getObjects()) {
            if (!o.properties.strings.contains("type")) {
                continue;
            }
            if (o.properties.strings.at("type") != "exit") {
                continue;
            }

            auto const pos = o.position;
            auto const distance = jt::MathHelper::distanceBetween(
                pos, m_player->getPosition() - GP::LevelTileSize() * 0.5f);
            if (distance
                <= jt::MathHelper::length(GP::LevelTileSize()) * 0.25f) {
                auto const new_level_name
                    = o.properties.strings.at("new_level_name");
                auto const door_id = o.properties.ints.at("door_id");
                std::cout << "trigger level change: " << new_level_name << "\n";
                loadLevel((std::filesystem::path { "assets" } / new_level_name)
                        .string());
                m_player->setPosition(m_level->getDoorPosition(door_id));
                getGame()->gfx().camera().setCamOffset(
                    m_player->getPosition() - GP::GetScreenSize() * 0.5f);
            }
        }

        updateLevelEncounters(elapsed);
    }

    m_background->update(elapsed);
    m_vignette->update(elapsed);
    m_medkitSprite->update(elapsed);
    m_molotovSprite->update(elapsed);
    m_medkitText->update(elapsed);
    m_molotovText->update(elapsed);
}

void StateGame::onDraw() const
{
    m_background->draw(renderTarget());
    m_stars->draw();
    drawObjects();

    m_vignette->draw();
    m_hud->draw();

    m_medkitSprite->draw(renderTarget());
    m_medkitText->draw(renderTarget());
    m_molotovSprite->draw(renderTarget());
    m_molotovText->draw(renderTarget());
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
