
#include "level.hpp"
#include "game_properties.hpp"

Level::Level(std::weak_ptr<jt::Box2DWorldInterface> world)
    : m_world { world }
{
}

void Level::doCreate() { }

void Level::doUpdate(float const elapsed)
{
    m_tileLayerGround->update(elapsed);
    m_tileLayerGroundDecals->update(elapsed);
    m_tileLayerOverhead->update(elapsed);
}

void Level::doDraw() const
{
    m_tileLayerGround->draw(renderTarget());
    m_tileLayerGroundDecals->draw(renderTarget());
    m_tileLayerOverhead->draw(renderTarget());
}

void Level::loadLevelCollisions(jt::tilemap::TilesonLoader& loader)
{
    auto tileCollisions = loader.loadCollisionsFromLayer("ground");

    tileCollisions.refineColliders(24);
    for (auto const& r : tileCollisions.getRects()) {
        auto bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_staticBody;
        bodyDef.position = b2Vec2 { r.left + r.width / 2.0f, r.top + r.height / 2.0f };

        auto shapeDef = b2DefaultShapeDef();
        shapeDef.material.friction = 0.2f;
        auto const boxCollider = b2MakeBox(r.width / 2.0f, r.height / 2.0f);

        auto collider = std::make_shared<jt::Box2DObject>(m_world.lock(), bodyDef);
        b2CreatePolygonShape(collider->getBodyId(), &shapeDef, &boxCollider);

        m_colliders.push_back(collider);
    }
}

void Level::loadTileLayers(jt::tilemap::TilesonLoader& loader)
{
    m_tileLayerGround = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("ground", textureManager(), "assets/"));
    m_tileLayerGround->setScreenSizeHint(GP::GetScreenSize());
    m_tileLayerGround->setZ(GP::LevelZLayerBelowPlayer());

    m_tileLayerGroundDecals = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("ground_decals", textureManager(), "assets/"));
    m_tileLayerGroundDecals->setScreenSizeHint(GP::GetScreenSize());
    m_tileLayerGroundDecals->setZ(GP::LevelZLayerBelowPlayer());

    m_tileLayerOverhead = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("overhead", textureManager(), "assets/"));
    m_tileLayerOverhead->setScreenSizeHint(GP::GetScreenSize());
    m_tileLayerOverhead->setZ(GP::LevelZLayerAbovePlayer());

    m_objectsLayer
        = std::make_shared<jt::tilemap::ObjectLayer>(loader.loadObjectsFromLayer("objects"));

    m_encountersLayer
        = std::make_shared<jt::tilemap::ObjectLayer>(loader.loadObjectsFromLayer("encounters"));
}

std::vector<jt::tilemap::InfoRect> Level::getObjects() const
{
    return m_objectsLayer->getObjects();
}

std::vector<jt::tilemap::InfoRect> Level::getEncounters() const
{
    return m_encountersLayer->getObjects();
}

std::vector<std::shared_ptr<jt::Box2DObject>>& Level::getColliders() { return m_colliders; }

jt::Vector2f Level::getPlayerStartingPos() const
{
    for (auto const& obj : m_objectsLayer->getObjects()) {
        auto const type = obj.properties.strings.at("type");
        if (type == "player_start") {

            return obj.position + GP::LevelTileSize() * 0.5;
        }
    }
    return jt::Vector2f { };
}
