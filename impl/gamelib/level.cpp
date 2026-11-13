
#include "level.hpp"

Level::Level(std::weak_ptr<jt::Box2DWorldInterface> world)
    : m_world { world }
{
}

void Level::doCreate() { }

void Level::doUpdate(float const elapsed)
{
    m_tileLayerGround->update(elapsed);
    m_tileLayerOverlay->update(elapsed);
}

void Level::doDraw() const
{
    m_tileLayerGround->draw(renderTarget());
    m_tileLayerOverlay->draw(renderTarget());
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
    m_tileLayerGround->setScreenSizeHint(jt::Vector2f { 400, 300 });

    m_tileLayerOverlay = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("overlay", textureManager(), "assets/"));
    
    m_tileLayerOverlay->setScreenSizeHint(jt::Vector2f { 400, 300 });

    m_itemsLayer = std::make_shared<jt::tilemap::ObjectLayer>(loader.loadObjectsFromLayer("items"));
}

std::vector<jt::tilemap::InfoRect> Level::getItemObjects() { return m_itemsLayer->getObjects(); }

std::vector<jt::tilemap::InfoRect> Level::getObjects() { return m_invObjectsLayer->getObjects(); }

jt::Vector2f Level::getPlayerStartingPos() const
{

    for (auto const& obj : m_invObjectsLayer->getObjects()) {
        auto const type = obj.properties.strings.at("type");
        if (type == "player_start") {
            return obj.position;
        }
    }
    return jt::Vector2f { };
}
